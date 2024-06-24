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
// Created by jilianyi<jilianyi@naishu.tech> on 2024/5/24.
//

//
// Created by ljz_w on 2024/5/22.
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

#include <complex>
#include <cmath>
#include <vector>
#include <iostream>
#include <numeric>

#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include "NSUKit.h"

#define TIME_WIDTH 10e-6
#define CHANNEL_NUM 6
#define BLOCK 60000*CHANNEL_NUM*10

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


//template<typename T>
//bool generate_signal(float timeWidth, uint32_t freq, uint32_t fs, bool isCos, T* buffer) {
//    uint32_t points = timeWidth * fs * sizeof(T);
//    double step = freq/(double )fs;
//    const uint32_t type_max = ((1 << (sizeof(T) * 8 - 1)) - 1);
//
//    double signal_alo = isCos?
//            [](float it){return cos(it);} : [](double it){return sin(it);};
//    for(uint32_t i=0; i<points; i++) {
//        buffer[i] = static_cast<T>(signal_alo(2 * M_PI * i * step)*type_max);
//    }
//    return true;
//}


/**
 * iq   I in high digits, Q in low digits
 * @tparam T
 * @param timeWidth 信号时宽
 * @param freq 信号频率
 * @param fs 采样率
 * @param buffer 缓存
 * @return
 */
template<typename T>
bool generate_signal_iq(float timeWidth, uint32_t freq, uint32_t fs, T* buffer) {
    uint32_t points = timeWidth * fs * sizeof(T)*2;
    double step = freq/(double )fs;
    const uint32_t type_max = ((1 << (sizeof(T) * 8 - 1)) - 1);

    for(uint32_t i=0; i<points; i=i+2) {
        buffer[i]   = static_cast<T>(cos(2 * M_PI * i * step)*type_max);
        buffer[i+1] = static_cast<T>(sin(2 * M_PI * i * step)*type_max);
    }
    return true;
}


/**
 * reverse iq   I in low digits, Q in high digits
 * @tparam T
 * @param timeWidth 信号时宽
 * @param freq 信号频率
 * @param fs 采样率
 * @param buffer 缓存
 * @return
 */
template<typename T>
bool generate_signal_iq_reverse(float timeWidth, uint32_t freq, uint32_t fs, T* buffer) {
    uint32_t points = timeWidth * fs * sizeof(T)*2;
    double step = freq/(double )fs;
    const uint32_t type_max = ((1 << (sizeof(T) * 8 - 1)) - 1);

    for(uint32_t i=0; i<points; i=i+2) {
        buffer[i]   = static_cast<T>(sin(2 * M_PI * i * step)*type_max);
        buffer[i+1] = static_cast<T>(cos(2 * M_PI * i * step)*type_max);
    }
    return true;
}


/**
 * 数据下行线程，从板卡中按粒度捞出数据，将数据推入full队列
 * @param _kit NSUSoc类的实例，与Soc进行数据流交互
 * @param q 循环队列
 * @param block 数据上行粒度
 * @param total 全部数据
 * @param mu 互斥锁
 * @param total_DDR_depth 总DDR深度
 */
void download_thread(nsukit::BaseKit *_kit, Deque *q, nsuSize_t block, nsuSize_t total, std::mutex *mu, nsuRegAddr_t total_DDR_depth) {

    std::unique_lock<std::mutex> *lock;
    nsuMemory_p mem;
    nsuSize_t current = 0;
    nsuRegAddr_t current_DDR_depth = 0;
    nsuRegValue_t *buff;


    while (true) {
        mem = q->empty.Pop();

        // 当DDR深度不足6MB时，叫停程序
//        _kit->read(0x00270000 + 0x9004, buff);
//        current_DDR_depth = *buff * 64;
//        while ((total_DDR_depth - current_DDR_depth) < block) {
//            _kit->read(0x00270000 + 0x9004, buff);
//            current_DDR_depth = *buff * 64;
//
//            // 睡眠 1ms
//            std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        }

        auto s = _kit->open_send(0, mem, block, 0);
        if (s != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            std::cout << "Establish CS and CR connections: "  << std::endl;
            std::cout << "Failed to enable data uplink " << nsukit::status2_string(s) << ", currently uplinked: " << current << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        s = nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
        while (s==nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING) {
            s = _kit->wait_stream(mem, 1.);
        }
        current += block;
        q->full.Push(mem);
//        if (current == total) break;
    }
    lock = new std::unique_lock<std::mutex>(*mu);
    q->stopFlag = 1;
    delete lock;
}

/**
 * 数据生成
 * @param _kit
 * @param q
 * @param block
 * @param mu
 * @param total_sampling_points 总采样点数
 * @param sampling_rate 采样频率
 * @param signal_frequency 信号频率
 */
void data_generate(nsukit::BaseKit *_kit, Deque *q, nsuSize_t block, std::mutex *mu,
                   uint32_t total_sampling_points, uint32_t sampling_rate, uint32_t signal_frequency)  {
    nsuMemory_p mem;
    int8_t *buffer;
    int cnt = 0;
    nsuSize_t speed_count;
    auto st = std::chrono::steady_clock::now();
    unsigned int current;
    int pack_len = 32;

    uint32_t points = TIME_WIDTH*sampling_rate*2;
    uint32_t points_align = std::ceil(points/(float)pack_len)*pack_len;
    auto signal = (int8_t *)malloc(points_align);
    generate_signal_iq_reverse<int8_t>(TIME_WIDTH, signal_frequency, sampling_rate, signal);
    uint32_t last_offset = 0;

    std::cout << TIME_WIDTH << " " << sampling_rate << " " << points_align*CHANNEL_NUM << " " << pack_len << std::endl;

    while (true) {
        {
            auto lock = std::unique_lock<std::mutex>(*mu);
            if (q->stopFlag == 1) break;
        }
        mem = q->full.Pop();
        buffer = (int8_t *)_kit->get_buffer(mem, block);
        current = 0;

        while(current+points_align*CHANNEL_NUM <= block) {  // memory iter
            uint32_t offset;
            for (offset=last_offset;offset<points_align;offset+=pack_len) {
                uint32_t mem_offset = current+offset*CHANNEL_NUM;
                for(int ch=0;ch<CHANNEL_NUM;ch++) {
                    memcpy(buffer+mem_offset+ch*pack_len, signal+offset, pack_len);
                }
            }
//            current += points_align * CHANNEL_NUM;
            current += (points_align-last_offset)*CHANNEL_NUM;
            last_offset = offset+pack_len;
            if(last_offset >= points_align) last_offset = 0;
        }
        if (current+points_align*CHANNEL_NUM > block) {
            int _len = block-current;
            uint32_t offset;
            for(offset=last_offset;offset<last_offset+_len/CHANNEL_NUM;offset+=pack_len) {
                uint32_t mem_offset = current+offset;
                for(int ch=0;ch<CHANNEL_NUM;ch++) {
                    memcpy(buffer+mem_offset+ch*pack_len, signal+offset, pack_len);
                }
                last_offset = offset+pack_len;
                if(last_offset >= points_align) last_offset = 0;
            }
        }
        q->empty.Push(mem);
        speed_count += block;

        if (cnt % 20 == 0) {
            auto count = std::chrono::steady_clock::now() - st;
            std::cout << std::flush << '\r' << "当前数据生成速度: " << speed_count*1000./(count.count()) << "MB/s" << std::endl;
//            speed_count = 0;
        }
    }
    free(signal);
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    Deque q;
    std::mutex mu;

    /* 初始化NSUSoc实例，采用PCIE相关协议 */
    nsukit::NSUSoc <nsukit::PCIECmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};

    if (argc != 5) {
        std::cout << "Unsupported parameter passing method" << std::endl;
        // ./DataDownloadRreverseStop DDR总深度 总采样点数 采样率 信号频率
        std::cout << argv[0] << " {DDR total depth} {total sampling points} {sampling rate} {signal frequency}" << std::endl;
        return 1;
    }

    nsuRegAddr_t total_DDR_depth = std::atoi(argv[1]); // DDR总深度，单位：B

    /* PCIe协议的初始化参数 */
    nsuInitParam_t param;
    param.stream_board = 0;

    int total_sampling_points = std::atoi(argv[2]); // 总采样点数
    int sampling_rate = std::atoi(argv[3]);         // 采样频率
    int signal_frequency = std::atoi(argv[4]);      // 信号频率
    // 包长与信号时宽的最小公倍数
    unsigned int ds_block = BLOCK;
//    unsigned int ds_block = std::lcm(
//            CHANNEL_NUM*32,
//            static_cast<int>(sampling_rate*TIME_WIDTH* sizeof(int8_t )*CHANNEL_NUM));


    // 根据总采样点数求数据总量
    int total_len = total_sampling_points * 2;

    /* 查看PCIe的方法 */
    //auto res = kit.link_cmd(&param);
    //if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
    //    std::cout << "Establish CS and CR connections: " << nsukit::status2_string(res) << std::endl;
    //}
    auto res = kit.link_stream(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "Establish a DS connection: " << nsukit::status2_string(res) << std::endl;
    }

    // 申请10个6MB的空间
    for (int i=0; i<10; i++) {
        nsuMemory_p mem = kit.alloc_buffer(ds_block);
        q.full.Push(mem);
    }

    // 通知FPGA开始采集
    //res = kit.execute("系统开启");
    //if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
    //    std::cout << "系统开启：" << nsukit::status2_string(res) << std::endl;
    //}

    // 线程
    // 线程A 开启下行
    std::thread up_trd(download_thread, &kit, &q, ds_block, total_len, &mu, total_DDR_depth);
    // 线程B 数据生成
    std::thread generate_trd(data_generate, &kit, &q, ds_block, &mu, total_sampling_points, sampling_rate, signal_frequency);

    up_trd.join();
    generate_trd.join();

    // 通知FPGA开始采集
    //res = kit.execute("系统停止");
    //if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
    //    std::cout << "系统停止：" << nsukit::status2_string(res) << std::endl;
    //}

    std::cout << "Data download completed" << std::endl;

    return 0;
}


