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
#include <chrono>
#include <iomanip>
#include <ctime>

#include "type.h"
#include "version.h"


namespace nsukit {

#ifdef NSUKIT_DEBUG_EN
#define DEBUG_ENABLED         true
#else
#define DEBUG_ENABLED         false
#endif

#define NSU_REG_BWIDTH        4

#define DEBUG_PRINT(message)  debugPrint("func", __func__, message)
#define DEBUG_PRINT_CLASS(message)  debugPrint(typeid(*this).name(), __func__, message)

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

    std::string NSU_DLLEXPORT api_info();

    std::string NSU_DLLEXPORT status2_string(nsukitStatus_t status);

}


inline void debugPrint(const std::string &className, const std::string &functionName, const std::string &message) {
#if DEBUG_ENABLED
    // 获取当前时间
    auto now = std::chrono::system_clock::now();

    // 转换时间为秒和毫秒
    auto time_since_epoch = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch - seconds);

    // 获取本地时间
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = localtime(&time_now);

    // 打印时间、调用函数信息和消息
    std::cout << "[" << std::put_time(local_time, "%F %T") << "." << std::setw(3) << std::setfill('0')
              << milliseconds.count() << "] ";
    std::cout << "DEBUG (" << className << "::" << functionName << "): " << message << std::endl;
#endif
}

#endif //NSUKIT_CONFIG_H
