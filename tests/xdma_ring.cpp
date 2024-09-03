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
// Created by jilianyi<jilianyi@naishu.tech> on 2024/8/14.
//

#include "test_config.h"
#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include "NSUKit.h"


#define SocType nsukit::NSUSoc <nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::PCIERingUItf>




/**
 * @class ThreadSafeQueue
 * @tparam T
 */
template<typename T>
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
        auto value = (T *) queue_.front();
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
void upload_thread(nsukit::BaseKit *_kit, Deque *q, nsuSize_t block, nsuSize_t total, std::mutex *mu) {
    std::unique_lock<std::mutex> *lock;
    nsuMemory_p mem;
    nsuSize_t current = 0;
    while (true) {
        mem = q->empty.Pop();
        auto s = _kit->open_recv(0, mem, block, 0);
        if (s != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            std::cout << "Establish CS and CR connections: " << std::endl;
            std::cout << "Failed to enable data uplink " << nsukit::status2_string(s) << ", currently uplinked: "
                      << current << std::endl;
            break;
        }
        s = nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
        while (s == nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING) {
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
        outF.write((char *) _kit->get_buffer(mem, block), block);
        q->empty.Push(mem);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
        speed_count += block;

        if (cnt % 20 == 0) {
            auto count = std::chrono::steady_clock::now() - st;
            std::cout << std::flush << '\r' << "current write file speed: " << speed_count * 1000. / (count.count())
                      << "MB/s" << std::endl;
            speed_count = 0;
            st = std::chrono::steady_clock::now();
        }
        cnt++;
    }
    std::cout << std::endl;
    outF.close();
}


TEST(PCIERingTest, BasicUse) {
unsigned int ds_block = 32 * 1024 * 1024;
Deque q;
std::mutex mu;
SocType *kit = new SocType{};
nsuSize_t total_len = 32 * 1024 * 1024 * 10;


nsuInitParam_t param;
param.cmd_board = 0;
param.stream_board = 0;
param.ring_chnl_size[0] = ds_block * 10;

auto res = kit->link_stream(&param);
if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
std::cout << "Establish a DS connection: " <<
nsukit::status2_string(res)
<<
std::endl;
}

std::cout << "SocLink Successful!!!   now alloc buffer" <<
std::endl;
for (
int i = 0;
i<10; i++) {
nsuMemory_p mem = kit->alloc_buffer(ds_block);
q.empty.
Push(mem);
}

// Start the upstream and storage threads.
std::thread up_trd(upload_thread, kit, &q, ds_block, total_len, &mu);
std::thread write_trd(write_file_thread, kit, &q, ds_block, "/media/data.dat", &mu);

std::cout << "Stream thread start successful!!!" << std::endl;

up_trd.join();
write_trd.join();

std::cout << "Data upload completed" <<std::endl;

}
