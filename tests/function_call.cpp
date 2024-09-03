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
// Created by jilianyi<jilianyi@naishu.tech> on 2024/9/1.
//
#include "test_config.h"
#include "NSUKit.h"
#include "high_level/FunctionCall.h"
#include <future>


//TEST(HighLevelTest, ParamConfig) {
//    auto kit = (nsukit::BaseKit *)nsukit::hl_fc_create_soc();
//
//    EXPECT_EQ(nsukit::hl_fc_link_cmd(kit, "127.0.0.1", 6002, "./icd.json"), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
//
//    auto res = nsukit::hl_fc_get_parami32(kit, "RFConfigResult");
//    char pname[100] = "RFConfigResult";
//    nsukit::hl_fc_set_param(kit, pname, 10);
//    res = nsukit::hl_fc_get_parami32(kit, pname);
//    std::cout << res << std::endl;
//}
//
//
//TEST(HighLevelTest, StreamRecv) {
//    auto kit = (nsukit::BaseKit *)nsukit::hl_fc_create_soc();
//
//    auto res = nsukit::hl_fc_link_stream(kit, 7001);
//    auto fd = nsukit::hl_fc_alloc_buffer(kit, 1024);
//    res = nsukit::hl_fc_stream_recv(kit, 0, fd, 1024, 0);
//    auto data = (uint32_t *)nsukit::hl_fc_get_buffer(kit, fd, 1024);
//    std::cout << data << std::endl;
//}