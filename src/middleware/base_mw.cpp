//
// Created by 56585 on 2023/8/15.
//

#include "middleware/base_mw.h"

using namespace nsukit;


uint8_t I_BaseRegMw::get_param(nsuCSParam_t &param_name, uint8_t _default) { return _default; }

int8_t I_BaseRegMw::get_param(nsuCSParam_t &param_name, int8_t _default) { return _default; }

uint16_t I_BaseRegMw::get_param(nsuCSParam_t &param_name, uint16_t _default) { return _default; }

int16_t I_BaseRegMw::get_param(nsuCSParam_t &param_name, int16_t _default) { return _default; }

uint32_t I_BaseRegMw::get_param(nsuCSParam_t &param_name, uint32_t _default) { return _default; }

int32_t I_BaseRegMw::get_param(nsuCSParam_t &param_name, int32_t _default) { return _default; }

double I_BaseRegMw::get_param(nsuCSParam_t &param_name, double _default) { return _default; }

float I_BaseRegMw::get_param(nsuCSParam_t &param_name, float _default) { return _default; }

std::string I_BaseRegMw::get_param(nsuCSParam_t &param_name, std::string _default) { return _default; }


nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const uint8_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const int8_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const uint16_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const int16_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const uint32_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const int32_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const double &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const float &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseRegMw::set_param(nsuCSParam_t &param_name, const std::string &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}


nsukitStatus_t I_BaseRegMw::execute(nsuCSName_t cname) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t
I_BaseStreamMw::open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t
I_BaseStreamMw::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseStreamMw::wait_stream(nsuMemory_p fd, float timeout) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t I_BaseStreamMw::break_stream(nsuMemory_p fd) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t
I_BaseStreamMw::stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                            bool (*stop_event)(), float timeout, int flag) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t
I_BaseStreamMw::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                            bool (*stop_event)(), float timeout, int flag) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}
