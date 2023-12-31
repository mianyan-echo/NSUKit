//
// Created by 56585 on 2023/8/16.
//
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

TEST(EnumTest, ToString) {
    EXPECT_EQ(status2_string(nsukitStatus_t::NSUKIT_STATUS_SUCCESS), "NSUKIT_STATUS_SUCCESS");
    std::cout << status2_string(
            nsukitStatus_t::NSUKIT_STATUS_SUCCESS | nsukitStatus_t::NSUKIT_STATUS_TIMEOUT) << std::endl;
}
