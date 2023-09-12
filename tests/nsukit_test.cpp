//
// Created by 56585 on 2023/8/15.
//
#include "test_config.h"
#include "NSUKit.h"

using namespace nsukit;


TEST(NSUKITTest, VersionCheck) {
    std::cout << "NSUKIT_PROJECT_VER == " << NSUKIT_PROJECT_VER << std::endl;
    std::cout << "NSUKIT_PROJECT_VER_CODE == " << NSUKIT_PROJECT_VER_CODE << std::endl;
    std::cout
    << "NSUKIT_VER(" << NSUKIT_PROJECT_VER_MAJOR << ", " << NSUKIT_PROJECT_VER_MINOR
    << ", " << NSUKIT_PROJECT_VER_PATCH << ") == "
    << NSUKIT_VER_(NSUKIT_PROJECT_VER_MAJOR, NSUKIT_PROJECT_VER_MINOR, NSUKIT_PROJECT_VER_PATCH)
    << std::endl;

    EXPECT_EQ(
            NSUKIT_VER_(NSUKIT_PROJECT_VER_MAJOR, NSUKIT_PROJECT_VER_MINOR, NSUKIT_PROJECT_VER_PATCH),
            NSUKIT_PROJECT_VER_CODE);
}


TEST(NSUKITTest, BaseTemplate) {
    NSUSoc<SimCmdUItf, SimCmdUItf, SimStreamUItf> kit;

    nsuSimParam_t param {};
    param.a = 10;
    // 基类被正常重载
    EXPECT_EQ(kit.link_cmd(&param), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // 输入空指针时可以正常报错
    EXPECT_EQ(kit.link_cmd(nullptr), nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE);

    // write重载
    EXPECT_EQ(kit.write(0x00000010, 30), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // read重载
    nsuRegValue_t res;
    EXPECT_EQ(kit.read(0x00000010, &res), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
}
