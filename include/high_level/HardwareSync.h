//
// Created by 56585 on 2023/12/27.
//

#ifndef NSUKIT_HARDWARESYNC_H
#define NSUKIT_HARDWARESYNC_H

#include "base/base_kit.h"


namespace nsukit {
#define HSYNC_DEFAULT_CMD_FMT "hsync_s1\nhsync_s2\nhsync_s3\nhsync_s4\nhsync_s5\nhsync_s6"
#define HSYNC_CMD_FMT_DELIM '\n'
#define HSYNC_EN_ASYNC   // 是否使能异步模式并发向各个slave发送hsync指令

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    /**
     *
     * @param master      主soc
     * @param slaves      从soc列表
     * @param slave_num   从soc的数量
     * @param cmd_str     同步流程要配置的六个指令名称，样例参照宏定义HSYNC_DEFAULT_CMD_FMT，分隔符为宏定义HSYNC_CMD_FMT_DELIM
     * @return 函数运行结果
     */
    hl_hsync_sync(nsukit::BaseKit *master, nsukit::BaseKit **slaves, int slave_num, char *cmd_str= nullptr);
}

#endif //NSUKIT_HARDWARESYNC_H
