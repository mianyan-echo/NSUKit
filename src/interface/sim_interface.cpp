//
// Created by 56585 on 2023/8/17.
//

#include "interface/sim_interface.h"

using namespace nsukit;

SimStreamUItf::SimStreamUItf() = default;

SimCmdUItf::SimCmdUItf() = default;


nsukitStatus_t SimCmdUItf::accept(nsuAcceptParam_t *param) {
    auto _param = (nsuSimParam_t *)param;
    sim_param_a = _param->a;
    sim_param_b = _param->b;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @return
 */
nsukitStatus_t SimCmdUItf::close() {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param s
 * @return
 */
nsukitStatus_t SimCmdUItf::set_timeout(int s) {
    simTimeout = s;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimCmdUItf::send_bytes(nsuBytes_t &bytes) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimCmdUItf::send_bytes(nsuCharBuf_p bytes) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimCmdUItf::recv_bytes(nsuSize_t size, nsuCharBuf_p buf) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param addr
 * @param value
 * @return
 */
nsukitStatus_t SimCmdUItf::write(nsuRegAddr_t addr, nsuRegValue_t value) {
    auto data = (nsuCharBuf_p)malloc(24);
    _fmt_reg_write(addr, value, data);
    free(data);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param addr
 * @param buf
 * @return
 */
nsukitStatus_t SimCmdUItf::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
    auto data = (nsuCharBuf_p)malloc(20);
    _fmt_reg_read(addr, data);
    free(data);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @return
 */
nsukitStatus_t SimStreamUItf::open_board() {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t SimStreamUItf::close() {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}
