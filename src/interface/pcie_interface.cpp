//
// Created by 56585 on 2023/8/15.
//

#include "interface/pcie_interface.h"

using namespace nsukit;


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


nsukitStatus_t PCIECmdUItf::send_bytes(nsuBytes_t& bytes) {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t PCIECmdUItf::send_bytes(nsuCharBuf_p bytes) {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t PCIECmdUItf::recv_bytes(nsuSize_t size, nsuCharBuf_p buf) {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param addr
 * @param value
 * @return
 */
nsukitStatus_t PCIECmdUItf::write(nsuRegAddr_t addr, nsuRegValue_t value) {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param addr
 * @param buf
 * @return
 */
nsukitStatus_t PCIECmdUItf::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @return
 */
nsukitStatus_t PCIEChnlUItf::open_board() {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t PCIEChnlUItf::close() {
    // TODO: 补充实现
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}
