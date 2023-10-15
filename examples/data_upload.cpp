/*
 * +---------------+                               +-------------------+
 * |               |       +---------------+       |                   |
 * |               |-----> |   full_queue  |-----> |                   |
 * |               |       +---------------+       |                   |
 * | upload_thread |                               | write_file_thread |
 * |               |       +---------------+       |                   |
 * |               |<----- |  empty_queue  |<----- |                   |
 * |               |       +---------------+       |                   |
 * +---------------+                               +-------------------+
 * @class ThreadSafeQueue  线程安全队列
 */
#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include "NSUKit.h"


/**
 * @class ThreadSafeQueue 线程安全队列
 * @tparam T
 */
template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {}

    // 向队列中插入元素
    void Push(T *value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        lock.unlock();
        condition_.notify_one();
    }

    // 从队列中弹出元素
    T *Pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty(); });
        auto value = (T *)queue_.front();
        queue_.pop();
        return value;
    }

private:
    std::queue<T *> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
};


typedef ThreadSafeQueue<void> memQueue;


struct Deque {
    memQueue full{};
    memQueue empty{};
    int stopFlag = 0;
};


/**
 * 数据上行线程，从板卡中按粒度捞出数据，将数据推入full队列
 * @param _kit NSUSoc类的实例，与Soc进行数据流交互
 * @param q 循环队列
 * @param block 数据上行粒度
 * @param total 全部数据
 * @param mu 互斥锁
 */
void upload_thread(nsukit::BaseKit *_kit, Deque *q, nsuSize_t block, nsuSize_t total, std::mutex *mu) {
    std::unique_lock<std::mutex> *lock;
    nsuMemory_p mem;
    nsuSize_t current = 0;
    while (true) {
        mem = q->empty.Pop();
        auto s = _kit->open_recv(0, mem, block, 0);
        if (s != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            std::cout << "建立CS、CR连接："  << std::endl;
            std::cout << "数据上行开启失败" << nsukit::status2_string(s) << ", 当前已上行：" << current << std::endl;
            break;
        }
        s = nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
        while (s==nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING) {
            s = _kit->wait_stream(mem, 1.);
        }
        current += block;
        q->full.Push(mem);
        if (current == total) break;
    }
    lock = new std::unique_lock<std::mutex>(*mu);
    q->stopFlag = 1;
    delete lock;
}


/**
 *
 * @param _kit
 * @param q
 * @param block
 * @param path
 * @param mu
 */
void write_file_thread(nsukit::BaseKit *_kit, Deque *q, nsuSize_t block, const std::string &path, std::mutex *mu) {
    std::unique_lock<std::mutex> *lock;
    nsuMemory_p mem;
    std::ofstream outF;
    outF.open(path, std::ofstream::binary);
    int cnt = 0;
    nsuSize_t speed_count;
    auto st = std::chrono::steady_clock::now();

    while (true) {
        lock = new std::unique_lock<std::mutex>(*mu);
        if (q->stopFlag == 1) {
            break;
        }
        delete lock;
        mem = q->full.Pop();

        outF.write((char *)_kit->get_buffer(mem, block), block);
        q->empty.Push(mem);
        speed_count += block;

        if (cnt % 20 == 0) {
            auto count = std::chrono::steady_clock::now() - st;
            std::cout << std::flush << '\r' << "当前写盘速度: " << speed_count*1000./(count.count()) << "MB/s" << std::endl;
//            speed_count = 0;
        }
    }
    std::cout << std::endl;
    outF.close();
}


int main(int argc, char *argv[]) {
    const int ds_block = 1024*1024;   // 1MB DS交互颗粒度
    Deque q;
    std::mutex mu;
    nsukit::NSUSoc <nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};

    if (argc != 4) {
        std::cout << "不受支持的传参方法" << std::endl;
        // DataUpload 127.0.0.1 104857600 ./da_data_1.dat
        std::cout << argv[0] << " {IP} {totalBytes} {filePath}" << std::endl;
        return 1;
    }
    nsuSize_t total_len = std::atoi(argv[2]);
    if (total_len % ds_block != 0) {
        std::cout << "上行数据总长度total_len " << total_len << "Bytes应为 " << ds_block << "Bytes的整倍数" << std::endl;
        return 1;
    }

    nsuInitParam_t param;
    param.cmd_ip = argv[1];
    param.cmd_board = 0;
    param.stream_board = 0;

    auto res = kit.link_cmd(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立CS、CR连接：" << nsukit::status2_string(res) << std::endl;
    }

    res = kit.link_stream(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立DS连接：" << nsukit::status2_string(res) << std::endl;
    }

    for (int i=0; i<10; i++) {
        // 准备DS连接要用的内存
        nsuMemory_p mem = kit.alloc_buffer(ds_block);                 // 申请内存
        q.empty.Push(mem);
    }


    // 通知FPGA开始采集
    res = kit.execute("系统开启");
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "系统开启：" << nsukit::status2_string(res) << std::endl;
    }

    // 开启上行及存盘线程
    std::thread up_trd(upload_thread, &kit, &q, ds_block, total_len, &mu);
    std::thread write_trd(write_file_thread, &kit, &q, ds_block, argv[3], &mu);

    up_trd.join();
    write_trd.join();

    // 通知FPGA开始采集
    res = kit.execute("系统停止");
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "系统停止：" << nsukit::status2_string(res) << std::endl;
    }

    std::cout << "数据落盘完成" << std::endl;

    return 0;
}