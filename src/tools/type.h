/**
 *
 */

#ifndef NSUKIT_TYPE_H
#define NSUKIT_TYPE_H

#ifdef linux

#define _API_CALL
#define DLLEXTERN extern "C"
#define DLLEXPORT
#else  //win, rtx

#define _API_CALL __stdcall
#define DLLEXTERN extern "C"
#define DLLEXPORT __declspec(dllexport)
#endif

#include <iostream>
#include <string>
#include <any>
#include <cstdarg>
#include <cstdint>
#include <vector>

/**
 * 统一描述接口返回的执行状态
 */
DLLEXTERN enum class nsukitStatus_t {
    NSUKIT_STATUS_SUCCESS          =                  0,        // 0 运行成功
    NSUKIT_STATUS_NEED_RELOAD      =                  1 << 0,   // 1 此方法需要重载
    NSUKIT_STATUS_ARCH_MISMATCH    =                  1 << 1,   // 2 不支持此操作系统
    NSUKIT_STATUS_ALLOC_FAILED     =                  1 << 2,   // 4 内存申请失败
    NSUKIT_STATUS_INVALID_VALUE    =                  1 << 3,   // 8
    NSUKIT_STATUS_TEMP_MISMATCH    =                  1 << 4
};

DLLEXPORT nsukitStatus_t operator |(nsukitStatus_t lhs, nsukitStatus_t rhs);

DLLEXPORT void operator|= (nsukitStatus_t &lhs, nsukitStatus_t rhs);


DLLEXTERN typedef         std::vector<char>                     nsuBytes_t;
DLLEXTERN typedef         char *                                nsuCharBuf_p;
DLLEXTERN typedef         void *                                nsuVoidBuf_p;
DLLEXTERN typedef         uint8_t                               nsuBoardNum_t;
DLLEXTERN typedef         uint8_t                               nsuChnlNum_t;
DLLEXTERN typedef         size_t *                              nsuMemory_p;
DLLEXTERN typedef         size_t                                nsuSize_t;
DLLEXTERN typedef         nsuSize_t                             nsuStreamLen_t;
DLLEXTERN typedef         uint32_t                              nsuRegAddr_t;
DLLEXTERN typedef         uint32_t                              nsuRegValue_t;
DLLEXTERN typedef         std::string                           nsuICDParam_t;
DLLEXTERN typedef         nsukitStatus_t                        nsuStatus_t;
DLLEXTERN typedef         nsukitStatus_t *                      nsuStatus_p;


/**
 * 各个interface的accept方法传参所用的基类
 */
struct DLLEXPORT nsuAcceptParam_t {
};


struct DLLEXPORT nsuTCPParam_t: nsuAcceptParam_t{
    std::string addr;
    uint16_t port{};
};


struct DLLEXPORT nsuXDMAParam_t: nsuAcceptParam_t{
    nsuBoardNum_t board = 0;
    nsuRegAddr_t sent_base=0;
    nsuRegAddr_t recv_base=0;
};

struct DLLEXPORT nsuSimParam_t: nsuAcceptParam_t{
    int a = 1;
    int b = 2;
};
#endif //NSUKIT_TYPE_H
