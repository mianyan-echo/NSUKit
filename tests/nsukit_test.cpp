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

    nsuInitParam_t param {};
    param.sim_target = 10;
    // 基类被正常重载
    EXPECT_EQ(kit.link_cmd(&param), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // 输入空指针时可以正常报错
    EXPECT_EQ(kit.link_cmd(nullptr), nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE);

    // write重载
    EXPECT_EQ(kit.write(0x00000010, 30), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // read重载
    nsuRegValue_t res;
    EXPECT_EQ(kit.read(0x00000010, &res), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // set_param重载
    EXPECT_EQ(kit.set_param("发送指令包头", 0xFF), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // get_param重载
    EXPECT_EQ(kit.get_param<uint32_t>("发送指令包头"), 0xFF);

    // execute
    kit.set_param("DDS配置结果", 0xFFFFFFFF);
    EXPECT_EQ(kit.execute("DDS配置"), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
    EXPECT_EQ(kit.get_param<uint32_t>("DDS配置结果"), 0);
}


TEST(NSUKITTest, DataUploadTest) {
    NSUSoc<SimCmdUItf, SimCmdUItf, SimStreamUItf> kit{};
    size_t buf_len = 1024*1024;

    nsuInitParam_t param;

    param.sim_target = 0;
    // 模拟生成数据函数，数据上行时，会向alloc_buffer中写入数据
    param.sim_stream_func = [](nsuCharBuf_p buf, nsuStreamLen_t len) {
        for (int i=1; i<len; i++) {
            *(buf+i-1) = (char )i;
        }
    };

    kit.link_cmd(&param);
    kit.link_stream(&param);

    auto fd = kit.alloc_buffer(buf_len);
    auto data_ptr = kit.get_buffer(fd, buf_len);

    // 模拟开启采集
    kit.write(0x10000000, 1);
    kit.write(0x10000000, 0);

    kit.stream_recv(0, fd, buf_len, 0);

    EXPECT_EQ(*(uint8_t *)data_ptr, 1);
}
