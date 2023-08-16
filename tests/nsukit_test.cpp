//
// Created by 56585 on 2023/8/15.
//

#define NSUKIT_IN_TEST

#include <gtest/gtest.h>
#include <ctime>
#include "NSUKit.h"


TEST(NSUKITTest, BaseTemplate) {
    NSUKit::NSUKit<PCIECmdUItf, PCIEChnlUItf> kit{};

    nsuXDMAParam_t param {};
    param.board = 0;
    auto status = kit.start_command(&param);
    // 基类被正常重载
    EXPECT_EQ(status, nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // 输入空指针时可以正常报错
    EXPECT_EQ(kit.start_command(nullptr), nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE);
}
