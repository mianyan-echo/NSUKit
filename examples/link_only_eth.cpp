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
// Created by jilianyi<jilianyi@naishu.tech> on 2024/4/19.
//

#include "NSUKit.h"

int main() {
    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::SimStreamUItf> kit{};
    nsuInitParam_t param;
    param.cmd_ip ="192.168.1.162";
    param.cmd_tcp_port = 5001;
    param.stream_ip ="192.168.1.162";
    param.stream_tcp_port = 6002;

    auto res = kit.link_cmd(&param);
    res = kit.link_stream(&param);

    return 0;
}

