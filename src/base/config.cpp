//
// Created by 56585 on 2023/8/14.
//

#include "base/config.h"


std::string nsukit::api_info() {
    return std::string("NSUKit: version: v") + std::string(NSUKIT_PROJECT_VER)
         + std::string("\nbuild_time: ") + std::string(NSUKIT_BUILD_TIME)
         + std::string("\ngit_info: ") + std::string(NSUKIT_GIT_INFO);
}


std::string nsukit::status2_string(nsukitStatus_t status) {
    auto res = std::string{};
    switch (status) {
        case nsukitStatus_t::NSUKIT_STATUS_SUCCESS:
            res += "&&NSUKIT_STATUS_SUCCESS";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD:
            res += "&&NSUKIT_STATUS_NEED_RELOAD";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_ARCH_MISMATCH:
            res += "&&NSUKIT_STATUS_ARCH_MISMATCH";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_ALLOC_FAILED:
            res += "&&NSUKIT_STATUS_ALLOC_FAILED";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE:
            res += "&&NSUKIT_STATUS_INVALID_VALUE";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_TEMP_MISMATCH:
            res += "&&NSUKIT_STATUS_TEMP_MISMATCH";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_MISMATCH_MIXIN:
            res += "&&NSUKIT_STATUS_MISMATCH_MIXIN";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_ACCEPT_FAIL:
            res += "&&NSUKIT_STATUS_ACCEPT_FAIL";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL:
            res += "&&NSUKIT_STATUS_STREAM_FAIL";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING:
            res += "&&NSUKIT_STATUS_STREAM_RUNNING";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_TIMEOUT:
            res += "&&NSUKIT_STATUS_TIMEOUT";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_MEMBER_NOT_SUPPORT:
            res += "&&NSUKIT_STATUS_MEMBER_NOT_SUPPORT";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL:
            res += "&&NSUKIT_STATUS_ITF_FAIL";
            break;
        case nsukitStatus_t::NSUKIT_STATUS_NOT_LINK:
            res += "&&NSUKIT_STATUS_NOT_LINK";
            break;
        default:
            res += "&&NSUKIT_UNKNOWN_STATUS";
            break;
    }
    return res.substr(2);
}
