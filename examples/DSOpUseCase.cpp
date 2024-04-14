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

#include <iostream>
#include <fstream>
#include "NSUKit.h"


int main(int argc, char *argv[]) {
    const int ds_chnl = 0;
    std::ofstream outf;
    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};
    nsuInitParam_t param;

    std::cout << "当前示例" << argv[0]
              << "为展示基于PCIE的数据上行的接口调用示例，功能为从板卡0的数传通道0上行指定数量的数据并写入文件" << std::endl;
    if (argc != 4) {
        std::cout << "不受支持的传参方法" << std::endl;
        std::cout << argv[0] << " {IP} {totalBytes} {filePath}" << std::endl;
        return 1;
    }
    nsuSize_t total_len = std::atoi(argv[2]);
    if (total_len % 4 != 0) {
        std::cout << "上行数据总长度total_len " << total_len << "Bytes应为 " << 4 << "Bytes的整倍数" << std::endl;
        return 1;
    }

    // 连接板卡
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

    // 准备DS连接要用的内存
    nsuMemory_p mem = kit.alloc_buffer(total_len);                 // 申请内存
    auto ds_buf = (char *)kit.get_buffer(mem, total_len);          // 获取内存首指针

    // 通知FPGA开始采集
    res = kit.execute("系统开启");
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "系统开启：" << nsukit::status2_string(res) << std::endl;
    }

    // 上行一个颗粒度的数据
    res = kit.open_recv(ds_chnl, mem, total_len, 0);
    if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "成功开启DS交互" << std::endl;
        auto ds_state = nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
        while (ds_state != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            ds_state = kit.wait_stream(mem, 1000.);
//            std::cout << "wait_stream：" << nsukit::status2_string(ds_state) << std::endl;
        }
    }

    std::cout << *(uint32_t *)ds_buf << std::endl;

    // 通知FPGA停止采集
    res = kit.execute("系统停止");
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "系统停止：" << nsukit::status2_string(res) << std::endl;
    }

    // 存盘
    outf.open(argv[3], std::ofstream::binary);
    outf.write(ds_buf, total_len);
    outf.close();

    kit.free_buffer(mem);  // 释放内存
    return 0;
}
