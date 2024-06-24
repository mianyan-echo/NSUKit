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
#include "high_level/HardwareSync.h"


typedef nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::SimStreamUItf> SocType;


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "不受支持的传参方法" << std::endl;
        std::cout << argv[0] << " {master_IP} {slave1_IP} {slave2_IP} ..." << std::endl;
        return -1;
    }

    SocType master{};
    nsukit::BaseKit* slaves[argc-2];
    nsuInitParam_t master_param;
    nsuInitParam_t slaves_param[argc-2];

    master_param.cmd_ip = argv[1];
    auto res = master.link_cmd(&master_param);
    for(int i=2;i<argc;i++) {
        auto s_kit = new SocType;
        slaves_param[i-2].cmd_ip = argv[i];
        std::cout << slaves_param[i-2].cmd_ip << std::endl;
        res |= s_kit->link_cmd(&(slaves_param[i-2]));
        slaves[i-2] = s_kit;
    }

    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立连接：" << nsukit::status2_string(res) << std::endl;
    }

    char cmd_fmt[] = "hsync_s1\nhsync_s2\nhsync_s3\nhsync_s4\nhsync_s5\nhsync_s6";
    res |= nsukit::hl_hsync_sync(&master, slaves, argc-2, cmd_fmt);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "多板同步：" << nsukit::status2_string(res) << std::endl;
    } else {
        std::cout << "Hardware Synchronization Succeeded" << std::endl;
    }

    for(int i=2;i<argc;i++) {
        delete slaves[i-2];
    }

    return 0;
}
