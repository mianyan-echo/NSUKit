//
// Created by 56585 on 2023/8/16.
//
#define NSUKIT_IN_TEST

#include "test_config.h"
#include "NSUKit.h"

using namespace nsukit;


TEST(EnumTest, BitWiseTest) {
    EXPECT_EQ(nsukitStatus_t::NSUKIT_STATUS_ALLOC_FAILED|nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD,
              nsukitStatus_t(0b101));

    EXPECT_EQ(nsukitStatus_t::NSUKIT_STATUS_SUCCESS|nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD,
              nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD);

    auto status = nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    status |= nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    EXPECT_EQ(status, nsukitStatus_t(0b1001));
}
