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
#include "NSUKit.h"

int main(int argc, char *argv[]) {
    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};

    if (argc != 3) {
        std::cout << "不受支持的传参方法" << std::endl;
        return 1;
    }
    nsuInitParam_t param;

    param.cmd_ip = argv[1];
    param.cmd_board = 0;
    param.stream_board = 0;
    auto res = kit.link_cmd(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立连接：" << nsukit::status2_string(res) << std::endl;
    }

    res |= kit.set_param("固件路径", argv[2]);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "设置参数：" << nsukit::status2_string(res) << std::endl;
    }

    res |= kit.execute("固件更新");
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "执行固件更新：" << nsukit::status2_string(res) << std::endl;
    }

    std::cout << "固件更新结果：" << kit.get_param<uint32_t>("固件更新结果") << std::endl;

    return 0;
}
