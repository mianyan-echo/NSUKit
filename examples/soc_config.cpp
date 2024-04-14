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
    nsuInitParam_t param;

    param.cmd_ip = argv[1];
    param.icd_path = "./icd.json";
    param.cmd_board = 0;
    param.stream_board = 0;
    auto res = kit.link_cmd(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立连接：" << nsukit::status2_string(res) << std::endl;
    }

    switch (argc) {
        case 2:
            // SocConfig {SocIP}
            res = kit.execute("RF配置");
            if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
                std::cout << "RF配置：" << nsukit::status2_string(res) << std::endl;
            }
            std::cout << "RF配置执行结果：" << kit.get_param<nsuRegValue_t>("RF配置结果") << std::endl;

            res = kit.execute("DDS配置");
            if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
                std::cout << "DDS配置：" << nsukit::status2_string(res) << std::endl;
            }
            std::cout << "DDS配置执行结果：" << kit.get_param<nsuRegValue_t>("DDS配置结果") << std::endl;

            res = kit.execute("奈奎斯特区配置");
            if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
                std::cout << "奈奎斯特区配置：" << nsukit::status2_string(res) << std::endl;
            }
            break;
        case 3:
            // SocConfig {SocIP} {CmdName}
            res = kit.execute(argv[2]);
            if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
                std::cout << argv[2] << ": " << nsukit::status2_string(res) << std::endl;
            }
            break;
        default:
            std::cout << "不受支持的传参方法" << std::endl;
            std::cout << argv[0] << " IP " << std::endl;
            std::cout << argv[0] << " IP cmdName" << std::endl;
    }

    return 0;
}