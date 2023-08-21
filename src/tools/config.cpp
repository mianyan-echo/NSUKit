//
// Created by 56585 on 2023/8/14.
//

#include "config.h"

using namespace nsukit;


/**
 * sleep函数，ms为单位
 * @param milliseconds sleep时间 ms
 */
void uSleep(DWORD milliseconds) {
    Sleep(milliseconds * s_unit);
}
