////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024. Naishu
// NSUKit is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan PSL v2.
// You may obtain a copy of Mulan PSL v2 at:
//          http://license.coscl.org.cn/MulanPSL2
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See the Mulan PSL v2 for more details.
////////////////////////////////////////////////////////////////////////////////

//
// Created by jilianyi<jilianyi@naishu.tech> on 2024/8/27.
//

#include "high_level/FunctionCall.h"
#include "NSUKit.h"


void * nsukit::hl_fc_create_soc(uint8_t type) {
    nsukit::BaseKit *res;
    if (type == 0) {
        res = new nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::TCPStreamUItf>();
    }

    return res;
}

void nsukit::hl_fc_release_soc(nsukit::BaseKit *kit) {
    if (kit != nullptr) {
        delete kit;
    }
}


nsukitStatus_t nsukit::hl_fc_link_cmd(nsukit::BaseKit *kit, const char *addr, uint32_t port, const char *icd_path) {
    nsuInitParam_t param;
    param.cmd_ip = addr;
    param.stream_ip = addr;
    param.icd_path = icd_path;
    param.cmd_tcp_port = port;
    return kit->link_cmd(&param);
}

nsukitStatus_t nsukit::hl_fc_unlink_cmd(nsukit::BaseKit *kit) {
    return kit->unlink_cmd();
}

nsukitStatus_t nsukit::hl_fc_link_stream(nsukit::BaseKit *kit, uint32_t port) {
    nsuInitParam_t param;
    param.stream_tcp_port = port;
    return kit->link_stream(&param);
}

nsukitStatus_t nsukit::hl_fc_unlink_stream(nsukit::BaseKit *kit) {
    return kit->unlink_stream();
}

nsukitStatus_t nsukit::hl_fc_set_parami32(nsukit::BaseKit *kit, const char* name, int32_t value) {
    return hl_fc_set_param<int32_t>(kit, name, value);
}

nsukitStatus_t nsukit::hl_fc_set_parami16(nsukit::BaseKit *kit, const char* name, int16_t value) {
    return hl_fc_set_param<int16_t>(kit, name, value);
}

nsukitStatus_t nsukit::hl_fc_set_parami8(nsukit::BaseKit *kit, const char* name, int8_t value) {
    return hl_fc_set_param<int8_t>(kit, name, value);
}

nsukitStatus_t nsukit::hl_fc_set_paramf32(nsukit::BaseKit *kit, const char* name, float value) {
    return hl_fc_set_param<float>(kit, name, value);
}

nsukitStatus_t nsukit::hl_fc_set_paramf64(nsukit::BaseKit *kit, const char* name, double value) {
    return hl_fc_set_param<double>(kit, name, value);
}

nsukitStatus_t nsukit::hl_fc_set_paramstr(nsukit::BaseKit *kit, const char* name, void* value) {
    auto val = std::string((char*)value);
    auto _name = std::string(name);
    auto res = kit->set_param(_name, val);
    return res;
}


int32_t nsukit::hl_fc_get_parami32(nsukit::BaseKit *kit, const char *name) {
    return hl_fc_get_param<int32_t>(kit, name);
}

int16_t nsukit::hl_fc_get_parami16(nsukit::BaseKit *kit, const char *name) {
    return hl_fc_get_param<int16_t >(kit, name);
}

int8_t nsukit::hl_fc_get_parami8(nsukit::BaseKit *kit, const char *name) {
    return hl_fc_get_param<int8_t >(kit, name);
}

float nsukit::hl_fc_get_paramf32(nsukit::BaseKit *kit, const char *name) {
    return hl_fc_get_param<float >(kit, name);
}

double nsukit::hl_fc_get_paramf64(nsukit::BaseKit *kit, const char *name) {
    return hl_fc_get_param<double >(kit, name);
}

char* nsukit::hl_fc_get_paramstr(nsukit::BaseKit *kit, const char *name) {
    std::string val;
    auto _name = std::string(name);
    val = kit->get_param(_name, val);
    auto res = (char *)malloc(val.size());
    memcpy(res, val.data(), val.size());
    return res;
}

nsukitStatus_t nsukit::hl_fc_execute(nsukit::BaseKit *kit, const char *name) {
    return kit->execute(name);
}

nsukitStatus_t nsukit::hl_fc_write(nsukit::BaseKit *kit, nsuRegAddr_t name, nsuRegValue_t value) {
    return kit->write(name, value);
}

nsuRegValue_t nsukit::hl_fc_read(nsukit::BaseKit *kit, nsuRegAddr_t name) {
    nsuRegValue_t value;
    kit->read(name, &value);
    return value;
}

nsuMemory_p nsukit::hl_fc_alloc_buffer(nsukit::BaseKit *kit, nsuStreamLen_t length, nsuVoidBuf_p buf) {
    return kit->alloc_buffer(length, buf);
}

nsukitStatus_t nsukit::hl_fc_free_buffer(nsukit::BaseKit *kit, nsuMemory_p fd) {
    return kit->free_buffer(fd);
}

nsuVoidBuf_p nsukit::hl_fc_get_buffer(nsukit::BaseKit *kit, nsuMemory_p fd, nsuStreamLen_t length) {
    return kit->get_buffer(fd, length);
}

nsukitStatus_t nsukit::hl_fc_stream_recv(nsukit::BaseKit *kit, nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length,
                                         nsuStreamLen_t offset) {
    return kit->stream_recv(chnl, fd, length, offset);
}

nsukitStatus_t nsukit::hl_fc_wait_stream(nsukit::BaseKit *kit, nsuMemory_p fd, float timeout) {
    return kit->wait_stream(fd, timeout);
}

nsukitStatus_t nsukit::hl_fc_break_stream(nsukit::BaseKit *kit, nsuMemory_p fd) {
    return kit->break_stream(fd);
}

nsukitStatus_t nsukit::hl_fc_open_recv(nsukit::BaseKit *kit, nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length,
                                       nsuStreamLen_t offset) {
    return kit->open_recv(chnl, fd, length, offset);
}

nsukitStatus_t nsukit::hl_fc_recv_to_file_useless(nsukit::BaseKit *kit, nsuChnlNum_t chnl, nsuMemory_p fd,
                                          nsuStreamLen_t length, const char *file_name) {
    auto res = hl_fc_stream_recv(kit, chnl, fd, length, 0);
    return res;
}

uint32_t nsukit::hl_fc_tool_get_first_point(nsuVoidBuf_p data_p) {
    return *(uint32_t *) data_p;
}
