//
// Created by 56585 on 2023/8/14.
//

#include "base/config.h"

using namespace nsukit;


std::string nsukit::api_info() {
    return std::string("NSUKit: version: v") + std::string(NSUKIT_PROJECT_VER)
         + std::string("\nbuild_time: ") + std::string(NSUKIT_BUILD_TIME)
         + std::string("\ngit_info: ") + std::string(NSUKIT_GIT_INFO);
}


/**
 * sleep函数，ms为单位
 * @param milliseconds sleep时间 ms
 */
void uSleep(DWORD milliseconds) {
    Sleep(milliseconds * s_unit);
}
