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
// Created by jilianyi<jilianyi@naishu.tech> on 2024/6/17.
//


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
 * @class ThreadSafeQueue
 */
#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include "NSUKit.h"


#define STREAM_WITH_PCIE
//#define STREAM_WITH_TCP
#define SocType nsukit::NSUSoc <nsukit::SimCmdUItf, nsukit::SimCmdUItf, nsukit::SimStreamUItf>

#ifdef STREAM_WITH_PCIE
#define SocType nsukit::NSUSoc <nsukit::PCIECmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf>
#endif

#ifdef STREAM_WITH_TCP
#define SocType nsukit::NSUSoc <nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::TCPStreamUItf>
#endif


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
 * Data upstream thread retrieves data from the board at a granular level and pushes it into the full queue.
 * @param _kit
 * @param q
 * @param block
 * @param total
 * @param mu
 */
void upload_thread(nsukit::BaseKit *_kit, int stream_chnl, Deque *q, nsuSize_t block, nsuSize_t total, std::mutex *mu) {
    std::unique_lock<std::mutex> *lock;
    nsuMemory_p mem;
    nsuSize_t current = 0;
    while (true) {
        mem = q->empty.Pop();
        auto s = _kit->open_recv(stream_chnl, mem, block, 0);
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
        if (current == total) {
            break;
        };
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
    nsuSize_t speed_count = 0;
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
//        std::this_thread::sleep_for(std::chrono::seconds(1));
        speed_count += block;

        if (cnt % 20 == 0) {
            auto count = std::chrono::steady_clock::now() - st;
            std::cout << std::flush << '\r' << "current write file speed: " << speed_count*1000./(count.count()) << "MB/s" << std::endl;
            speed_count = 0;
            st = std::chrono::steady_clock::now();
        }
        cnt++;
    }
    std::cout << std::endl;
    outF.close();
}


int aurora_reset(nsukit::BaseKit *kit) {
    kit->write(0x50010, 1);
    kit->write(0x50010, 0);
    kit->write(0x51010, 1);
    kit->write(0x51010, 0);
    kit->write(0x52010, 1);
    kit->write(0x52010, 0);
    kit->write(0x53010, 1);
    kit->write(0x53010, 0);
    kit->write(0x0, 0);
    kit->write(0x0, 1);
    return 0;
}


int ddr_reset(nsukit::BaseKit *kit) {
    kit->write(0x00004, 0);
    kit->write(0x00004, 1);
    kit->write(0x00008, 0);
    kit->write(0x00008, 1);
    kit->write(0x0000C, 0);
    kit->write(0x0000C, 1);
    kit->write(0x00010, 0);
    kit->write(0x00010, 1);
    kit->write(0x2001C, 0);
    kit->write(0x2001C, 1);
    kit->write(0x2001C, 2);
    kit->write(0x2001C, 3);
    kit->write(0x20014, 0);
    kit->write(0x20014, 1);
    kit->write(0x20010, 0);
    kit->write(0x20010, 1);
    kit->write(0x20020, 0xF);
    kit->write(0x20020, 0);
    kit->write(0x00004, 0);
    kit->write(0x00004, 1);
    kit->write(0x00008, 0);
    kit->write(0x00008, 1);
    kit->write(0x0000C, 0);
    kit->write(0x0000C, 1);
    kit->write(0x00010, 0);
    kit->write(0x00010, 1);
    return  0;
}


int source_reset(nsukit::BaseKit *kit) {
    kit->write(0x30000, 0x0);
    kit->write(0x31000, 0x0);
    kit->write(0x32000, 0x0);
    kit->write(0x33000, 0x0);
    kit->write(0x30008, 0x0);
    kit->write(0x30008, 0x1);
    kit->write(0x31008, 0x0);
    kit->write(0x31008, 0x1);
    kit->write(0x32008, 0x0);
    kit->write(0x32008, 0x1);
    kit->write(0x33008, 0x0);
    kit->write(0x33008, 0x1);
    return  0;
}


int source_select(nsukit::BaseKit *kit) {
    kit->write(0x24   , 0x1);                    // 通道0切换至aurora   1：数据来自采集设备  0：数据由板卡内部产生
    kit->write(0x30030, 98304);                  // 通道0打包长度       内部产生测试数据打包长度
    kit->write(0x3000C, 49024);                  // 通道0递增数上限      内部产生递增测试数据最大值，数据达到最大值之后归零递增
    kit->write(0x30004, 100);                    // 通道0带宽设置        单位MB/s，产生实时数据带宽
    kit->write(0x30010, 0x0);                    // 通道0常数使能        1：产生常数数据，与“递增数据使能”互斥，只能其中一个配置为1
    kit->write(0x3002C, 0xAAAA0000);             // 通道0通道标识号
    kit->write(0x3003C, 0x10);                   // 通道0数据位宽       16bit位宽数据
    kit->write(0x30000, 0x1);                    // 通道0递增数使能      1：产生递增数据
    kit->write(0x28   , 0x1);                    // 通道1切换至aurora
    kit->write(0x31030, 0x4000);                 // 通道1打包长度
    kit->write(0x3100C, 0x1F80);                 // 通道1递增数上限
    kit->write(0x31004, 0x190);                  // 通道1带宽设置
    kit->write(0x31010, 0x0);                    // 通道1常数使能
    kit->write(0x3102C, 0xAAAA1111);             // 通道1通道标识号
    kit->write(0x3103C, 0x10);                   // 通道1数据位宽
    kit->write(0x31000, 0x1);                    // 通道1递增数使能
    kit->write(0x2c   , 0x1);                    // 通道2切换至aurora
    kit->write(0x32030, 0x4000);                 // 通道2打包长度
    kit->write(0x3200C, 0x1F80);                 // 通道2递增数上限
    kit->write(0x32004, 0x190);                  // 通道2带宽设置
    kit->write(0x32010, 0x0);                    // 通道2常数使能
    kit->write(0x3202C, 0xAAAA2222);             // 通道2通道标识号
    kit->write(0x3203C, 0x10);                   // 通道2数据位宽
    kit->write(0x32000, 0x1);                    // 通道2递增数使能
    kit->write(0x30   , 0x1);                    // 通道3切换至aurora
    kit->write(0x33030, 0x4000);                 // 通道3打包长度
    kit->write(0x3300C, 0x1F80);                 // 通道3递增数上限
    kit->write(0x33004, 0x190);                  // 通道3带宽设置
    kit->write(0x33010, 0x0);                    // 通道3常数使能
    kit->write(0x3302C, 0xAAAA3333);             // 通道3通道标识号
    kit->write(0x3303C, 0x10);                   // 通道3数据位宽
    kit->write(0x33000, 0x1);                    // 通道3递增数使能
    return  0;
}


int start_chnl(nsukit::BaseKit *kit, Deque *q, uint32_t stream_chnl, uint32_t ds_block, uint32_t total_len, std::string file_name, std::mutex *mu) {
    for (int i=0; i<10; i++) {
        nsuMemory_p mem = kit->alloc_buffer(ds_block);
        q->empty.Push(mem);
    }

    // Start the upstream and storage threads.
    std::thread up_trd(upload_thread, kit, stream_chnl, q, ds_block, total_len, mu);
    std::thread write_trd(write_file_thread, kit, q, ds_block, file_name, mu);

    std::cout << "Stream thread start successful!!!" << std::endl;

    up_trd.join();
    write_trd.join();
    return 0;
}


int main(int argc, char *argv[]) {
    unsigned int ds_block = 4*1024*1024;
    Deque q0, q1, q2, q3;
    std::mutex mu0, mu1, mu2, mu3;
    SocType kit{};

    if (argc != 2) {
        std::cout << "Unsupported parameter passing method" << std::endl;
        // DataUpload 104857600
        std::cout << argv[0] << " {totalBytes}" << std::endl;
        return 1;
    }
    nsuSize_t total_len = std::atoi(argv[1]);
    if (total_len % ds_block != 0) {
        std::cout << "The total length of upstream data total_len "
                  << total_len << " Bytes should be "
                  << ds_block << "Integer multiple of Bytes" << std::endl;
        return 1;
    }

    nsuInitParam_t param;
    param.cmd_board = 0;
    param.stream_board = 0;
#ifdef STREAM_WITH_TCP
    auto ip = std::string(argv[1]);
    // 192.168.1.161 => 6001
    std::string port_str{};
    port_str += ip[ip.length()-2];
    port_str += "00";
    port_str += ip[ip.length()-1];
    int port = std::atoi(port_str.data());
    param.stream_ip = ip;
    param.stream_tcp_port = port;
    param.stream_tcp_block = 4 * 1024 * 1024;
#endif

    auto res = kit.link_cmd(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "Establish CS and CR connections: " << nsukit::status2_string(res) << std::endl;
    }

    res = kit.link_stream(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "Establish a DS connection: " << nsukit::status2_string(res) << std::endl;
    }

    // config to start
    aurora_reset(&kit);
    ddr_reset(&kit);
    source_reset(&kit);
    source_select(&kit);

    std::cout << "SocLink Successful!!!   now alloc buffer" << std::endl;
    for (int i=0; i<10; i++) {
        nsuMemory_p mem = kit.alloc_buffer(ds_block);
        q0.empty.Push(mem);
    }

    for (int i=0; i<10; i++) {
        nsuMemory_p mem = kit.alloc_buffer(ds_block);
        q1.empty.Push(mem);
    }

    for (int i=0; i<10; i++) {
        nsuMemory_p mem = kit.alloc_buffer(ds_block);
        q2.empty.Push(mem);
    }

    for (int i=0; i<10; i++) {
        nsuMemory_p mem = kit.alloc_buffer(ds_block);
        q3.empty.Push(mem);
    }

    // Start the upstream and storage threads.
    std::thread up_trd_0(upload_thread, &kit, 0, &q0, ds_block, total_len, &mu0);
    std::thread write_trd_0(write_file_thread, &kit, &q0, ds_block, "data_ch_0.dat", &mu0);
    std::thread up_trd_1(upload_thread, &kit, 1, &q1, ds_block, total_len, &mu1);
    std::thread write_trd_1(write_file_thread, &kit, &q1, ds_block, "data_ch_1.dat", &mu1);
    std::thread up_trd_2(upload_thread, &kit, 2, &q2, ds_block, total_len, &mu2);
    std::thread write_trd_2(write_file_thread, &kit, &q2, ds_block, "data_ch_2.dat", &mu2);
    std::thread up_trd_3(upload_thread, &kit, 3, &q3, ds_block, total_len, &mu3);
    std::thread write_trd_3(write_file_thread, &kit, &q3, ds_block, "data_ch_3.dat", &mu3);

    std::cout << "Stream thread start successful!!!" << std::endl;

    up_trd_0.join();
    write_trd_0.join();
    up_trd_1.join();
    write_trd_1.join();
    up_trd_2.join();
    write_trd_2.join();
    up_trd_3.join();
    write_trd_3.join();

    // config to stop
    aurora_reset(&kit);
    ddr_reset(&kit);
    source_reset(&kit);

    std::cout << "Data upload completed" << std::endl;

    return 0;
}

