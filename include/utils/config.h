//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_CONFIG_H
#define NSUKIT_CONFIG_H

#include <iostream>
#include <string>
#include <cstring>
#include <any>
#include <map>
#include <mutex>
#include <cstdarg>
#include <cstdint>
#include <vector>
#include <ranges>
#include <cmath>
#include "type.h"


#ifdef linux
#include <unistd.h>
#define Sleep usleep
#define s_unit 1000
typedef unsigned int DWORD;

#elif _WIN32

#include <winsock2.h>  // 在windows.h之前
#include <windows.h>

#define Sleep              Sleep
#define s_unit             1
#define NSU_REG_BWIDTH     4

#endif

namespace nsukit {
    /**
     * 前向声明BaseKit类，作为NSUKit类的基类使用
     */
    DLLEXTERN class NSU_DLLEXPORT BaseKit;


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


    extern void uSleep(DWORD milliseconds);
}

#endif //NSUKIT_CONFIG_H
