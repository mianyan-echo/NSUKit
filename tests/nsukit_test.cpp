//
// Created by 56585 on 2023/8/15.
//

#define NSUKIT_IN_TEST

#include <gtest/gtest.h>
#include <ctime>
#include "NSUKit.h"

using namespace nsukit;


TEST(NSUKITTest, BaseTemplate) {
    NSUKit<SimCmdUItf, SimChnlUItf> kit;

    nsuSimParam_t param {};
    param.a = 10;
    // 基类被正常重载
    EXPECT_EQ(kit.start_command(&param), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // 输入空指针时可以正常报错
    EXPECT_EQ(kit.start_command(nullptr), nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE);

    // write重载
    EXPECT_EQ(kit.write(0x00000010, 30), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // read重载
    nsuRegValue_t res;
    EXPECT_EQ(kit.read(0x00000010, &res), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
}
