//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_CONFIG_H
#define NSUKIT_CONFIG_H

#include <iostream>
#include <string>
#include <any>
#include <map>
#include <vector>

#include <cmath>
#include <cstring>

#include <thread>
#include <mutex>
#include <chrono>

#include <cstdarg>
#include <cstdint>

#include "type.h"
#include "version.h"


namespace nsukit {
#define NSU_REG_BWIDTH     4

    NSU_DLLEXPORT std::string api_info();

    NSU_DLLEXPORT std::string status2_string(nsukitStatus_t status);

    /**
     * 标准指令反馈
     */
    struct StandardCmdFeedback {
        uint32_t packHead = 0;
        uint32_t packId = 0;
        uint32_t packNum = 0;
        uint32_t packLength = 0;
        uint32_t execSucceed = 0;
    };
}

#endif //NSUKIT_CONFIG_H
