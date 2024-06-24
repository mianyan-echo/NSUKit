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
    auto buf = (nsuRegValue_t *) malloc(ceil_num* sizeof(nsuRegValue_t));
    for (int i=0;i<ceil_num;i++){
        res |= cmd_itf_->read(addr+reg_len*i, buf+i);
    }
    memcpy(value, buf, length);
    free(buf);
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
    auto buf = (nsuRegValue_t *) malloc(ceil_num*sizeof(nsuRegValue_t));
    memcpy(buf, value, length);
    for (int i=0;i<ceil_num;i++){
        res |= cmd_itf_->read(addr, buf+i);
    }
    free(buf);
    return res;
}


nsuSize_t Mixin_VirtualRegCmd::_fmt_reg_read(nsuRegAddr_t reg, nsuCharBuf_p buf) {
    static constexpr uint32_t DataPacketID = 0x31001001;
    static constexpr size_t DataPacketSize = sizeof(RegPack) - sizeof(nsuRegValue_t);

    if (!buf) {
        return -1;  // 缓冲区为空，返回错误状态
    }

    RegPack pack;
    pack.id = DataPacketID;
    pack.length = DataPacketSize;
    pack.addr = reg;
    memcpy(buf, &pack, DataPacketSize);

    return DataPacketSize;
}


nsuSize_t Mixin_VirtualRegCmd::_fmt_reg_write(nsuRegAddr_t reg, nsuRegValue_t value, nsuCharBuf_p buf) {
    static constexpr uint32_t DataPacketID = 0x31001000;
    static constexpr size_t DataPacketLength = sizeof(RegPack);

    if (buf == nullptr) {
        return -1;  // 缓冲区为空，返回错误
    }

    RegPack pack;
    pack.id = DataPacketID;
    pack.length = DataPacketLength;
    pack.addr = reg;
    pack.value = value;

    memcpy(buf, &pack, sizeof(RegPack));
    return DataPacketLength;  // 数据格式化成功
}


nsukitStatus_t Mixin_VirtualRegCmd::_common_write(nsuRegAddr_t reg, nsuRegValue_t value) {
    auto buf = (nsuCharBuf_p) malloc(sizeof(RegPack));
    auto len = _fmt_reg_write(reg, value, buf);
    auto res = cmd_itf_->send_bytes(buf, len);
    free(buf);

    char r_head[16];
    RegPack pack;
    res |= cmd_itf_->recv_bytes(16, r_head);
    memcpy(&pack, r_head, 16);
    res |= cmd_itf_->recv_bytes(pack.length-16, r_head);
    if (*(nsuRegValue_t *)r_head!=0) {
        res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
        DEBUG_PRINT_CLASS("cmd result!=0");
    }
    return res;
}

nsukitStatus_t Mixin_VirtualRegCmd::_common_read(nsuRegAddr_t reg, nsuRegValue_t *buf) {
    if (buf == nullptr) return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    auto cmd_buf = (nsuCharBuf_p) malloc(sizeof(RegPack));
    auto len = _fmt_reg_read(reg, cmd_buf);
    auto res = cmd_itf_->send_bytes(cmd_buf, len);
    free(cmd_buf);

    char r_head[16];
    RegPack pack;
    res |= cmd_itf_->recv_bytes(16, r_head);
    memcpy(&pack, r_head, 16);
    res |= cmd_itf_->recv_bytes(pack.length-16, r_head);
    if (*(nsuRegValue_t *)r_head!=0) {
        res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
        DEBUG_PRINT_CLASS("cmd result!=0");
    }
    if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) *buf = *(nsuRegValue_t *)r_head;
    return res;
}


nsukitStatus_t Mixin_VirtualRegCmd::multi_write(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t> &value) {
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

nsukitStatus_t Mixin_VirtualRegCmd::multi_read(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t *> &value) {
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
Mixin_VirtualRegCmd::increment_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len) {
    auto padding_len = (nsuRegValue_t)(std::ceil((float )length/(float )reg_len)*(float)reg_len);
    RegPack head = {0x5F5F5F5F, 0x31001010, 0x00000000, padding_len+6*4, addr, padding_len};
    auto buf = (nsuCharBuf_p ) malloc(sizeof(RegPack)+padding_len);
    memcpy(buf, &head, sizeof(RegPack));
    memcpy(buf+sizeof(RegPack), value, length);
    if (padding_len > length) memset(buf+sizeof(RegPack)+length, 0, padding_len-length);

    auto res = cmd_itf_->send_bytes(buf, sizeof(RegPack)+padding_len);
    free(buf);

    char r_head[16];
    res |= cmd_itf_->recv_bytes(16, r_head);
    memcpy(&head, r_head, 16);
    res |= cmd_itf_->recv_bytes(head.length-16, r_head);
    if (*(nsuRegValue_t *)r_head!=0) {
        res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
    }
    return res;
}

nsukitStatus_t
Mixin_VirtualRegCmd::increment_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len) {
    auto padding_len = (nsuRegValue_t)(std::ceil((float )length/(float )reg_len)*(float)reg_len);
    RegPack head = {0x5F5F5F5F, 0x31001011, 0x00000000, 24, addr, padding_len};
    auto res = cmd_itf_->send_bytes((nsuCharBuf_p )&head, sizeof(RegPack));

    auto buf = (nsuCharBuf_p ) malloc(sizeof(RegPack)+padding_len);
    res |= cmd_itf_->recv_bytes(sizeof(RegPack)+padding_len-4, buf);
    if (*(nsuRegValue_t *)(buf+16)!=0) {
        res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
        DEBUG_PRINT_CLASS("cmd result!=0");
    }
    memcpy(value, buf+4*20, padding_len);
    free(buf);
    return res;
}

nsukitStatus_t
Mixin_VirtualRegCmd::loop_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len) {
    auto padding_len = (nsuRegValue_t)(std::ceil((float )length/(float )reg_len)*(float)reg_len);
    RegPack head = {0x5F5F5F5F, 0x31001020, 0x00000000, padding_len+6*4, addr, padding_len};
    auto buf = (nsuCharBuf_p ) malloc(sizeof(RegPack)+padding_len);
    memcpy(buf, &head, sizeof(RegPack));
    memcpy(buf+sizeof(RegPack), value, length);
    if (padding_len > length) memset(buf+sizeof(RegPack)+length, 0, padding_len-length);

    auto res = cmd_itf_->send_bytes(buf, sizeof(RegPack)+padding_len);
    free(buf);

    char r_head[16];
    res |= cmd_itf_->recv_bytes(16, r_head);
    memcpy(&head, r_head, 16);
    res |= cmd_itf_->recv_bytes(head.length-16, r_head);
    if (*(nsuRegValue_t *)r_head!=0) {
        res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
        DEBUG_PRINT_CLASS("cmd result!=0");
    }
    return res;
}

nsukitStatus_t
Mixin_VirtualRegCmd::loop_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len) {
    auto padding_len = (nsuRegValue_t)(std::ceil((float )length/(float )reg_len)*(float)reg_len);
    RegPack head = {0x5F5F5F5F, 0x31001021, 0x00000000, 24, addr, padding_len};
    auto res = cmd_itf_->send_bytes((nsuCharBuf_p )&head, sizeof(RegPack));

    auto buf = (nsuCharBuf_p ) malloc(sizeof(RegPack)+padding_len);
    res |= cmd_itf_->recv_bytes(sizeof(RegPack)+padding_len-4, buf);
    if (*(nsuRegValue_t *)(buf+16)!=0) {
        res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
        DEBUG_PRINT_CLASS("cmd result!=0");
    }
    memcpy(value, buf+20, padding_len);
    free(buf);
    return res;
}
