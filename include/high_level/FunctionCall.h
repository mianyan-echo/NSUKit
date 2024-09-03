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

#ifndef NSUKIT_FUNCTIONCALL_H
#define NSUKIT_FUNCTIONCALL_H

#include "base/base_kit.h"


namespace nsukit {
    DLLEXTERN void NSU_DLLEXPORT
    *hl_fc_create_soc(uint8_t type=0);

    DLLEXTERN void NSU_DLLEXPORT
    hl_fc_release_soc(nsukit::BaseKit *kit);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_link_cmd(nsukit::BaseKit *kit, const char* addr, uint32_t port, const char* icd_path);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_unlink_cmd(nsukit::BaseKit *kit);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_link_stream(nsukit::BaseKit *kit, uint32_t port);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_unlink_stream(nsukit::BaseKit *kit);

    template<typename T>
    nsukitStatus_t hl_fc_set_param(nsukit::BaseKit *kit, const char* name, T value) {
        auto _name = std::string(name);
        auto res = kit->set_param(_name, value);
        return res;
    }

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_set_parami32(nsukit::BaseKit *kit, const char* name, int32_t value);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_set_parami16(nsukit::BaseKit *kit, const char* name, int16_t value);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_set_parami8(nsukit::BaseKit *kit, const char* name, int8_t value);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_set_paramf32(nsukit::BaseKit *kit, const char* name, float value);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_set_paramf64(nsukit::BaseKit *kit, const char* name, double value);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_set_paramstr(nsukit::BaseKit *kit, const char* name, void* value);

    template<typename T>
    T hl_fc_get_param(nsukit::BaseKit *kit, const char* name) {
        T val = 0;
        auto _name = std::string(name);
        val = kit->get_param(_name, val);
        return val;
    }

    DLLEXTERN int32_t NSU_DLLEXPORT
    hl_fc_get_parami32(nsukit::BaseKit *kit, const char* name);

    DLLEXTERN int16_t NSU_DLLEXPORT
    hl_fc_get_parami16(nsukit::BaseKit *kit, const char* name);

    DLLEXTERN int8_t NSU_DLLEXPORT
    hl_fc_get_parami8(nsukit::BaseKit *kit, const char* name);

    DLLEXTERN float NSU_DLLEXPORT
    hl_fc_get_paramf32(nsukit::BaseKit *kit, const char* name);

    DLLEXTERN double NSU_DLLEXPORT
    hl_fc_get_paramf64(nsukit::BaseKit *kit, const char* name);

    DLLEXTERN char NSU_DLLEXPORT
    *hl_fc_get_paramstr(nsukit::BaseKit *kit, const char* name);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_execute(nsukit::BaseKit *kit, const char* name);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_write(nsukit::BaseKit *kit, nsuRegAddr_t name, nsuRegValue_t value);

    DLLEXTERN nsuRegValue_t NSU_DLLEXPORT
    hl_fc_read(nsukit::BaseKit *kit, nsuRegAddr_t name);

    DLLEXTERN nsuRegValue_t NSU_DLLEXPORT
    hl_fc_read(nsukit::BaseKit *kit, nsuRegAddr_t name);

    DLLEXTERN nsuMemory_p NSU_DLLEXPORT
    hl_fc_alloc_buffer(nsukit::BaseKit *kit, nsuStreamLen_t length, nsuVoidBuf_p buf = nullptr);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_free_buffer(nsukit::BaseKit *kit, nsuMemory_p fd);

    DLLEXTERN nsuVoidBuf_p NSU_DLLEXPORT
    hl_fc_get_buffer(nsukit::BaseKit *kit, nsuMemory_p fd, nsuStreamLen_t length=0);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_stream_recv(nsukit::BaseKit *kit, nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_wait_stream(nsukit::BaseKit *kit, nsuMemory_p fd, float timeout = 0.);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_break_stream(nsukit::BaseKit *kit, nsuMemory_p fd);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_open_recv(nsukit::BaseKit *kit, nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_fc_recv_to_file_useless(nsukit::BaseKit *kit, nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, const char* file_name);

    DLLEXTERN uint32_t NSU_DLLEXPORT
    hl_fc_tool_get_first_point(nsuVoidBuf_p data_p);
}

#endif //NSUKIT_FUNCTIONCALL_H
