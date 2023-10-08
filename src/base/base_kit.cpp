#include "base/base_kit.h"

using namespace nsukit;


uint8_t BaseKit::get_param(nsuCSParam_t &param_name, uint8_t _default) { return _default; }

int8_t BaseKit::get_param(nsuCSParam_t &param_name, int8_t _default) { return _default; }

uint16_t BaseKit::get_param(nsuCSParam_t &param_name, uint16_t _default) { return _default; }

int16_t BaseKit::get_param(nsuCSParam_t &param_name, int16_t _default) { return _default; }

uint32_t BaseKit::get_param(nsuCSParam_t &param_name, uint32_t _default) { return _default; }

int32_t BaseKit::get_param(nsuCSParam_t &param_name, int32_t _default) { return _default; }

double BaseKit::get_param(nsuCSParam_t &param_name, double _default) { return _default; }

float BaseKit::get_param(nsuCSParam_t &param_name, float _default) { return _default; }

std::string BaseKit::get_param(nsuCSParam_t &param_name, std::string _default) { return _default; }


nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const uint8_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const int8_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const uint16_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const int16_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const uint32_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const int32_t &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const double &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const float &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}

nsukitStatus_t BaseKit::set_param(nsuCSParam_t &param_name, const std::string &value) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}


nsukitStatus_t BaseKit::execute(nsuCSName_t cname) {
    return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
}
