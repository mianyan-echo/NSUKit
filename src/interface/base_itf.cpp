//
// Created by 56585 on 2023/8/14.
//

#include "base_itf.h"

using namespace nsukit;

/**
 *
 * @param reg
 * @param buf
 * @return
 */
nsukitStatus_t I_BaseCmdUItf::_fmt_reg_read(nsuRegAddr_t reg, nsuCharBuf_p buf) {
    static constexpr uint32_t DataPacketID = 0x31000000;
    static constexpr size_t DataPacketSize = sizeof(RegPack) + sizeof(nsuRegAddr_t);

    if (!buf) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;  // 缓冲区为空，返回错误状态
    }

    RegPack pack;
    pack.id = DataPacketID;
    pack.length = DataPacketSize;
    memcpy(buf, &pack, sizeof(RegPack));
    // 直接进行内存拷贝，避免逐个字节复制
    memcpy(buf+sizeof(pack), &reg, sizeof(reg));

    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param reg
 * @return
 */
nsuBytes_t I_BaseCmdUItf::_fmt_reg_read(nsuRegAddr_t reg) {
    nsuBytes_t res;
    RegPack pack;
    char _res[sizeof(RegPack)+sizeof(nsuRegAddr_t)];
    pack.id = 0x31000000;
    pack.length = 20;
    memcpy(_res, &pack, sizeof(RegPack));
    memcpy(_res+ sizeof(RegPack), &reg, sizeof(nsuRegAddr_t));

    for(auto ch: _res) {
        res.push_back(ch);
    }
    return res;
}
