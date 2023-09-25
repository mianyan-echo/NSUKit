//
// Created by 56585 on 2023/8/14.
//

#include "interface/base_itf.h"

using namespace nsukit;


nsukitStatus_t Mixin_NativeRegCmd::multi_write(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t> &value) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    if (addr.size() != value.size()) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto _a = addr.begin();
    auto _v = value.begin();

    while (_a != addr.end() && _v != value.end()) {
        res |= cmd_itf_->write(*_a, *_v);
        _a++;
        _v++;
    }
    return res;
}


nsukitStatus_t Mixin_NativeRegCmd::multi_read(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t *> &value) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    if (addr.size() != value.size()) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto _a = addr.begin();
    auto _v = value.begin();

    while (_a != addr.end() && _v != value.end()) {
        res |= cmd_itf_->read(*_a, *_v);
        _a++;
        _v++;
    }
    return res;
}


nsukitStatus_t
Mixin_NativeRegCmd::increment_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    auto num = length / reg_len;
    auto ptr = (nsuRegValue_t *) value;
    for (int i=0;i<num;i++){
        res |= cmd_itf_->write(addr+reg_len*i, *(ptr+i));
    }
    if ((length - num*reg_len) > 0) {
        nsuRegValue_t last = 0;
        memcpy(&last, ptr+num, length - num*reg_len);
        res |= cmd_itf_->write(addr+reg_len*num, last);
    }
    return res;
}


nsukitStatus_t
Mixin_NativeRegCmd::increment_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    if (value == nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto ceil_num = (nsuSize_t )std::ceil((float )length/(float )reg_len);
    nsuRegValue_t buf[ceil_num];
    memcpy(buf, value, length);
    for (int i=0;i<ceil_num;i++){
        res |= cmd_itf_->read(addr+reg_len*i, buf+i);
    }
    return res;
}


nsukitStatus_t
Mixin_NativeRegCmd::loop_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    auto num = length / reg_len;
    auto ptr = (nsuRegValue_t *) value;
    for (int i=0;i<num;i++){
        res |= cmd_itf_->write(addr, *(ptr+i));
    }
    if ((length - num*reg_len) > 0) {
        nsuRegValue_t last = 0;
        memcpy(&last, ptr+num, length - num*reg_len);
        res |= cmd_itf_->write(addr, last);
    }
    return res;
}


nsukitStatus_t
Mixin_NativeRegCmd::loop_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    if (value == nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto ceil_num = (nsuSize_t )std::ceil((float )length/(float )reg_len);
    nsuRegValue_t buf[ceil_num];
    memcpy(buf, value, length);
    for (int i=0;i<ceil_num;i++){
        res |= cmd_itf_->read(addr, buf+i);
    }
    return res;
}


/**
 *
 * @param reg
 * @param buf
 * @return
 */
nsukitStatus_t Mixin_VirtualRegCmd::_fmt_reg_read(nsuRegAddr_t reg, nsuCharBuf_p buf) {
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
nsuBytes_t Mixin_VirtualRegCmd::_fmt_reg_read(nsuRegAddr_t reg) {
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
