/**
 * @brief ....
 * @typedef nsuBytes_t sdfsdfs
 */

#ifndef NSUKIT_TYPE_H
#define NSUKIT_TYPE_H

#ifdef linux

#define _API_CALL
#define DLLEXTERN
#define NSU_DLLEXPORT
#else  //win, rtx

#define _API_CALL __stdcall
#define DLLEXTERN extern "C"
#define NSU_DLLEXPORT __declspec(dllexport)
#endif

#include <iostream>
#include <string>
#include <cstring>
#include <any>
#include <cstdarg>
#include <cstdint>
#include <vector>

/**
 * @enum nsukitStatus_t
 * 统一描述接口返回的执行状态
 */
DLLEXTERN enum class nsukitStatus_t {
    NSUKIT_STATUS_SUCCESS             =                  0,        /*< 0 运行成功 */
    NSUKIT_STATUS_NEED_RELOAD         =                  1 << 0,   // 1 此方法需要重载
    NSUKIT_STATUS_ARCH_MISMATCH       =                  1 << 1,   // 2 不支持此操作系统
    NSUKIT_STATUS_ALLOC_FAILED        =                  1 << 2,   // 4 内存申请失败
    NSUKIT_STATUS_INVALID_VALUE       =                  1 << 3,   // 8
    NSUKIT_STATUS_TEMP_MISMATCH       =                  1 << 4,
    NSUKIT_STATUS_MISMATCH_MIXIN      =                  1 << 5,
    NSUKIT_STATUS_ACCEPT_FAIL         =                  1 << 6,
    NSUKIT_STATUS_STREAM_FAIL         =                  1 << 7,
    NSUKIT_STATUS_STREAM_RUNNING      =                  1 << 8,
    NSUKIT_STATUS_TIMEOUT             =                  1 << 9,
    NSUKIT_STATUS_MEMBER_NOT_SUPPORT  =                  1 << 10,
    NSUKIT_STATUS_ITF_FAIL            =                  1 << 11
};

NSU_DLLEXPORT nsukitStatus_t operator |(nsukitStatus_t lhs, nsukitStatus_t rhs);

NSU_DLLEXPORT void operator|= (nsukitStatus_t &lhs, nsukitStatus_t rhs);


DLLEXTERN enum class nsuBulkMode {
    LOOP = 0,
    INCREMENT = 1
};


DLLEXTERN typedef         std::vector<char>                     nsuBytes_t;
DLLEXTERN typedef         char *                                nsuCharBuf_p;
DLLEXTERN typedef         void *                                nsuVoidBuf_p;
DLLEXTERN typedef         uint8_t                               nsuBoardNum_t;
DLLEXTERN typedef         uint8_t                               nsuChnlNum_t;
DLLEXTERN typedef         void *                                nsuMemory_p;
DLLEXTERN typedef         size_t                                nsuSize_t;
DLLEXTERN typedef         nsuSize_t                             nsuStreamLen_t;
DLLEXTERN typedef         uint32_t                              nsuRegAddr_t;
DLLEXTERN typedef         uint32_t                              nsuRegValue_t;
DLLEXTERN typedef         std::string                           nsuCSParam_t;
DLLEXTERN typedef         std::string                           nsuCSName_t;
DLLEXTERN typedef         nsukitStatus_t                        nsuStatus_t;
DLLEXTERN typedef         nsukitStatus_t *                      nsuStatus_p;


struct nsuTCPParam_t{
    std::string cmd_ip = "127.0.0.1";
    uint32_t cmd_tcp_port = 5001;
    float cmd_tcp_timeout = 1.;

    std::string stream_ip = "127.0.0.1";
    uint32_t stream_tcp_port{};
};


struct nsuSerialParam_t{
    std::string cmd_serial_port = "/dev/ttyUSB0";
    uint32_t cmd_baud_rate = 115200;
};


struct nsuXDMAParam_t{
    nsuBoardNum_t cmd_board = 0;
    nsuRegAddr_t cmd_sent_base=0;
    nsuRegAddr_t cmd_recv_base=0;
    nsuRegAddr_t cmd_irq_base=0;
    nsuRegAddr_t cmd_sent_down_base=0;

    nsuBoardNum_t stream_board=0;
};

/**
 * @struct nsuSimParam_t
 */
struct NSU_DLLEXPORT nsuSimParam_t {
    int sim_target = 1;         /*< 模拟目标 */
    void (*sim_stream_func)(nsuCharBuf_p buf, nsuSize_t length) =
            [](nsuCharBuf_p buf, nsuSize_t length) { memset(buf, 1, length); };    /*< 向输入的内存中写入模拟数据的函数 */
};


struct nsuMwParam_t{
    enum class StreamMode {
        REAL = 0,
        VIRTUAL = 1
    };
    std::string icd_path = "./icd.json";
    bool check_cs_recv = false;

    StreamMode stream_mode = StreamMode::REAL;
};


/**
 * 各个interface的accept方法传参所用的结构体
 */
struct NSU_DLLEXPORT nsuInitParam_t: nsuTCPParam_t, nsuSerialParam_t, nsuXDMAParam_t, nsuSimParam_t, nsuMwParam_t {};
#endif //NSUKIT_TYPE_H
