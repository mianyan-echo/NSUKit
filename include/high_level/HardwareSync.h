//!
// Created by 56585 on 2023/12/27.
//

#ifndef NSUKIT_HARDWARESYNC_H
#define NSUKIT_HARDWARESYNC_H

#include "base/base_kit.h"


namespace nsukit {
#define HSYNC_DEFAULT_CMD_FMT "hsync_s1\nhsync_s2\nhsync_s3\nhsync_s4\nhsync_s5\nhsync_s6"
#define HSYNC_CMD_FMT_DELIM '\n'
//#define HSYNC_EN_ASYNC   // 是否使能异步模式并发向各个slave发送hsync指令

    /**
     *
     * @param master      主soc
     * @param slaves      从soc列表
     * @param slave_num   从soc的数量
     * @param cmd_str     可选参数
     *                    同步流程要配置的六个指令名称，默认值为宏定义HSYNC_DEFAULT_CMD_FMT，分隔符为宏定义HSYNC_CMD_FMT_DELIM
     *                    当只配置了一条指令时，则会对master与slaves执行同样的指令，对应所有soc独立同步的情况
     * @return 函数运行结果
     * @retval nsukitStatus_t::NSUKIT_STATUS_SUCCESS 同步成功
     * @retval nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE cmd_str参数不符合规则
     *
     * @code
     * #include "NSUKit.h"
     * void main() {
     *     nsukit::Basekit * slaves[5];   // 5个slave soc
     *     nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};  // master soc
     *     nsuInitParam_t param;
     *     param.cmd_ip = "xxx.xxx.xxx.xxx";
     *     param.cmd_board = 0;
     *     param.stream_board = 0;
     *     kit.link_cmd(&param);
     *
     *     auto res = hl_hsync_sync(kit, slaves, 5);
     * }
     * @endcode
     */
    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_hsync_sync(nsukit::BaseKit *master, nsukit::BaseKit **slaves, int slave_num, char *cmd_str= nullptr);

}

#endif //NSUKIT_HARDWARESYNC_H
