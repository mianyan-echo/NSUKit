//
// Created by 56585 on 2023/8/16.
//

#include "middleware/virtual_chnl.h"

using namespace nsukit;


uint16_t VirtualChnlMw::VCHNL_NUM = 8;
uint16_t VirtualChnlMw::R2V_CHNL = 0;
nsuRegAddr_t VirtualChnlMw::PARAM_ADDR = 0x00000000;
nsuRegAddr_t VirtualChnlMw::PARAM_WR_ADDR = 0x00000000;
nsuRegAddr_t VirtualChnlMw::STATUS_ADDR = 0x00000000;


VirtualChnlMw::VirtualChnlMw(I_BaseStreamUItf *itf) : ds_itf(itf) {

}


nsukitStatus_t VirtualChnlMw::config(nsuInitParam_t *param) {
    streamMode = param->stream_mode;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t
VirtualChnlMw::open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    if (streamMode == StreamMode::REAL) return ds_itf->open_send(chnl, fd, length, offset);
    return nsukitStatus_t::NSUKIT_STATUS_MEMBER_NOT_SUPPORT;
}


nsukitStatus_t
VirtualChnlMw::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    if (streamMode == StreamMode::REAL) return ds_itf->open_recv(chnl, fd, length, offset);
    return nsukitStatus_t::NSUKIT_STATUS_MEMBER_NOT_SUPPORT;
}


nsukitStatus_t VirtualChnlMw::wait_stream(nsuMemory_p fd, float timeout) {
    if (streamMode == StreamMode::REAL) return ds_itf->wait_stream(fd, timeout);
    return nsukitStatus_t::NSUKIT_STATUS_MEMBER_NOT_SUPPORT;
}


nsukitStatus_t VirtualChnlMw::break_stream(nsuMemory_p fd) {
    if (streamMode == StreamMode::REAL) return ds_itf->break_stream(fd);
    return nsukitStatus_t::NSUKIT_STATUS_MEMBER_NOT_SUPPORT;
}


nsukitStatus_t
VirtualChnlMw::stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    if (streamMode == StreamMode::REAL) return ds_itf->stream_recv(chnl, fd, length, offset, stop_event, timeout, flag);
    return nsukitStatus_t::NSUKIT_STATUS_MEMBER_NOT_SUPPORT;
}


nsukitStatus_t
VirtualChnlMw::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    if (streamMode == StreamMode::REAL) return ds_itf->stream_send(chnl, fd, length, offset, stop_event, timeout, flag);
    return nsukitStatus_t::NSUKIT_STATUS_MEMBER_NOT_SUPPORT;
}
