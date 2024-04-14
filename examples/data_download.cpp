////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024. Naishu
// NSUKit is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan PSL v2.
// You may obtain a copy of Mulan PSL v2 at:
//          http://license.coscl.org.cn/MulanPSL2
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See the Mulan PSL v2 for more details.
////////////////////////////////////////////////////////////////////////////////

//
// Created by jilianyi<jilianyi@naishu.tech> on 2024/4/12.
//

/*
 * +-----------------+                                    +-------------------+
 * |                 |       +---------------+            |                   |
 * |                 |-----> |   full_queue  |---- ptr -->|                   |
 * |                 |       +---------------+            |                   |
 * | download_thread |                                    |   data_generate   |
 * |                 |       +---------------+            |                   |
 * |                 |<----- |  empty_queue  |<--- ptr ---|                   |
 * |                 |       +---------------+            |                   |
 * +-----------------+                                    +-------------------+
 * @class ThreadSafeQueue
 */
#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include "NSUKit.h"


/**
 * @class ThreadSafeQueue
 * @tparam T
 */
template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {}

    //
    void Push(T *value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        lock.unlock();
        condition_.notify_one();
    }

    //
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
 * 数据下行线程，从板卡中按粒度捞出数据，将数据推入full队列
 * @param _kit NSUSoc类的实例，与Soc进行数据流交互
 * @param q 循环队列
 * @param block 数据上行粒度
 * @param total 全部数据
 * @param mu 互斥锁
 */
void download_thread(nsukit::BaseKit *_kit, Deque *q, nsuSize_t block, nsuSize_t total, std::mutex *mu) {
    std::unique_lock<std::mutex> *lock;
    nsuMemory_p mem;
    nsuSize_t current = 0;
    while (true) {
        mem = q->empty.Pop();
        auto s = _kit->open_send(0, mem, block, 0);
        if (s != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            std::cout << "Establish CS and CR connections: "  << std::endl;
            std::cout << "Failed to enable data uplink " << nsukit::status2_string(s) << ", currently uplinked: " << current << std::endl;
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
 * @param mu
 */
void data_generate(nsukit::BaseKit *_kit, Deque *q, nsuSize_t block, std::mutex *mu) {
    std::unique_lock<std::mutex> *lock;
    nsuMemory_p mem;
    unsigned int *buffer;
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
        buffer = (unsigned int *)_kit->get_buffer(mem, block);
        for(int i=0; i<block; i++) buffer[i] = i;
        q->empty.Push(mem);
        speed_count += block;

        if (cnt % 20 == 0) {
            auto count = std::chrono::steady_clock::now() - st;
            std::cout << std::flush << '\r' << "当前数据生成速度: " << speed_count*1000./(count.count()) << "MB/s" << std::endl;
//            speed_count = 0;
        }
    }
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    unsigned int ds_block = 1024*1024;
    Deque q;
    std::mutex mu;
    nsukit::NSUSoc <nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};

    if (argc != 4) {
        std::cout << "Unsupported parameter passing method" << std::endl;
        // DataUpload 127.0.0.1 104857600 ./da_data_1.dat
        std::cout << argv[0] << " {IP} {totalBytes} {filePath}" << std::endl;
        return 1;
    }
    nsuSize_t total_len = std::atoi(argv[2]);
    if (total_len % ds_block != 0) {
        std::cout << "The total length of upstream data total_len "
                  << total_len << " Bytes should be "
                  << ds_block << " Integer multiple of Bytes" << std::endl;
        return 1;
    }

    nsuInitParam_t param;
    param.cmd_ip = argv[1];
    param.cmd_board = 0;
    param.stream_board = 0;

    auto res = kit.link_cmd(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "Establish CS and CR connections: " << nsukit::status2_string(res) << std::endl;
    }

    res = kit.link_stream(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "Establish a DS connection: " << nsukit::status2_string(res) << std::endl;
    }

    for (int i=0; i<10; i++) {
        nsuMemory_p mem = kit.alloc_buffer(ds_block);
        q.empty.Push(mem);
    }


    // 通知FPGA开始采集
    res = kit.execute("系统开启");
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "系统开启：" << nsukit::status2_string(res) << std::endl;
    }

    // 开启上行及存盘线程
    std::thread up_trd(download_thread, &kit, &q, ds_block, total_len, &mu);
    std::thread generate_trd(data_generate, &kit, &q, ds_block, &mu);

    up_trd.join();
    generate_trd.join();

    // 通知FPGA开始采集
    res = kit.execute("系统停止");
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "系统停止：" << nsukit::status2_string(res) << std::endl;
    }

    std::cout << "Data download completed" << std::endl;

    return 0;
}
