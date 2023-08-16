//
// Created by 56585 on 2023/8/15.
//

#include "pcie_interface.h"

PCIEChnlUItf::PCIEChnlUItf() = default;

PCIECmdUItf::PCIECmdUItf() = default;


/**
 *
 * @param board 板卡号
 * @param sent_base ICDOverAXI发送基地址
 * @param recv_base ICDOverAXI接收基地址
 * @return 是否连接成功
 */
nsukitStatus_t PCIECmdUItf::accept(nsuAcceptParam_t *param) {
    auto _param = (nsuXDMAParam_t *)param;
    pciBoard = _param->board;
    sentBase = _param->sent_base;
    recvBase = _param->recv_base;
    // TODO: 连接板卡
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @return
 */
nsukitStatus_t PCIECmdUItf::close() {
    // TODO: 与板卡断开链接
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param s
 * @return
 */
nsukitStatus_t PCIECmdUItf::set_timeout(int s) {
    pciTimeout = s;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t PCIECmdUItf::write(std::any addr, std::any value) {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

std::any PCIECmdUItf::read(std::any addr) {
    // TODO: 补充实现
    return I_BaseCmdUItf::read(addr);
}

nsukitStatus_t PCIECmdUItf::send_bytes() {
    // TODO: 补充实现
    return I_BaseCmdUItf::send_bytes();
}

nsukitStatus_t PCIECmdUItf::recv_bytes() {
    // TODO: 补充实现
    return I_BaseCmdUItf::recv_bytes();
}
