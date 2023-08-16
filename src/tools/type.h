//
// Created by 56585 on 2023/8/15.
//

#ifndef NSUKIT_TYPE_H
#define NSUKIT_TYPE_H

#include <iostream>
#include <string>
#include <any>
#include <cstdarg>
#include <cstdint>
#include <vector>

/**
 * 统一描述接口返回状态
 */
enum class nsukitStatus_t {
    NSUKIT_STATUS_SUCCESS          =                  0,        // 0 运行成功
    NSUKIT_STATUS_NEED_RELOAD      =                  1 << 0,   // 1 此方法需要重载
    NSUKIT_STATUS_ARCH_MISMATCH    =                  1 << 1,   // 2 不支持此操作系统
    NSUKIT_STATUS_ALLOC_FAILED     =                  1 << 2,   // 4 内存申请失败
    NSUKIT_STATUS_INVALID_VALUE    =                  1 << 3    // 8
};

nsukitStatus_t operator |(nsukitStatus_t lhs, nsukitStatus_t rhs);

void operator|= (nsukitStatus_t &lhs, nsukitStatus_t rhs);


typedef         std::vector<char>                     nsuBytes_t;
typedef         char *                                nsuCharBuf_p;
typedef         void *                                nsuVoidBuf_p;
typedef         uint8_t                               nsuBoardNum_t;
typedef         uint8_t                               nsuChnlNum_t;
typedef         unsigned long long *                  nsuMemory_p;
typedef         unsigned long long                    nsuStreamLen_t;
typedef         uint32_t                              nsuRegAddr_t;
typedef         uint32_t                              nsuRegValue_t;
typedef         std::string                           nsuICDParam_t;
typedef         nsukitStatus_t                        nsuStatus_t;
typedef         nsukitStatus_t *                      nsuStatus_p;


/**
 * 各个interface的accept方法传参所用的基类
 */
struct nsuAcceptParam_t {
};


struct nsuTCPParam_t: nsuAcceptParam_t{
    std::string addr;
    uint16_t port;
};


struct nsuXDMAParam_t: nsuAcceptParam_t{
    nsuBoardNum_t board = 0;
    nsuRegAddr_t sent_base=0;
    nsuRegAddr_t recv_base=0;
};
#endif //NSUKIT_TYPE_H
