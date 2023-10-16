//
// Created by 56585 on 2023/10/10.
//
#include <iostream>
#include "NSUKit.h"

int main(int argc, char *argv[]) {
    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};
    nsuInitParam_t param;

    param.cmd_ip = argv[1];
    param.cmd_board = 0;
    param.stream_board = 0;
    auto res = kit.link_cmd(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立连接：" << nsukit::status2_string(res) << std::endl;
    }

    return 0;
}