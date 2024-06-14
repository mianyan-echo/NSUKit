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
// Created by liuwen<liuwen@naishu.tech> on 2024/6/12.
//
#include "NSUKit.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Unsupported parameter passing method" << std::endl;
        std::cout << argv[0] << " {command_name}" << std::endl;
        return 1;
    }
    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::TCPStreamUItf> kit{};
    nsuInitParam_t param;
    param.cmd_ip = "192.168.1.201";
    param.cmd_tcp_port = 5001;
    auto res = kit.link_cmd(&param);
    std::cout << "command_name is " << argv[1] << std::endl;
    kit.execute(argv[1]);
}
