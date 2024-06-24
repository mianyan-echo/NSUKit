//
// Created by 56585 on 2023/8/15.
//
#include "test_config.h"
#include "NSUKit.h"

using namespace nsukit;


//TEST(NSUKITTest, ETH) {
//    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::TCPStreamUItf> kit{};
//    nsuInitParam_t param;
//    param.cmd_ip ="192.168.1.162";
//    param.cmd_tcp_port = 5001;
//    param.stream_ip ="192.168.1.162";
//    param.stream_tcp_port = 6002;
//
//    auto res = kit.link_cmd(&param);
//    res = kit.link_stream(&param);
//
////    res = kit.execute("RFConfig");
//    kit.set_param("ADC数据输出方式", 1);
//
//    auto mem = kit.alloc_buffer(10240);
//    auto mem_1 = kit.alloc_buffer(10240);
//    res = kit.execute("SocStart");
//    kit.stream_recv(0, mem, 1024, 0);
//    kit.stream_recv(0, mem_1, 1024, 1024);
//    auto buffer = kit.get_buffer(mem, 1024);
//}


//TEST(NSUKITTest, TTT) {
//    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};
//    nsuInitParam_t param;
//    int argc = 2;
//
//    param.cmd_ip = "192.168.1.152";
//    param.icd_path = "./icd.json";
//    param.cmd_board = 0;
//    param.stream_board = 0;
//    auto res = kit.link_cmd(&param);
//    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
//        std::cout << "建立连接：" << nsukit::status2_string(res) << std::endl;
//    }
//
//    switch (argc) {
//        case 2:
//            res = kit.execute("RF配置");
//            if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
//                std::cout << "RF配置：" << nsukit::status2_string(res) << std::endl;
//                std::cout << "RF配置执行结果：" << kit.get_param<nsuRegValue_t>("RF配置结果") << std::endl;
//            }
//
//            res = kit.execute("DDS配置");
//            if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
//                std::cout << "DDS配置：" << nsukit::status2_string(res) << std::endl;
//            }
//
//            res = kit.execute("奈奎斯特区配置");
//            if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
//                std::cout << "奈奎斯特区配置：" << nsukit::status2_string(res) << std::endl;
//            }
//        default:
//            std::cout << "不受支持的传参方法" << std::endl;
//    }
//
////    return 0;
//}


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
    // Base classes are overloaded normally

    EXPECT_EQ(kit.link_cmd(&param), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // When entering a null pointer, an error can be reported normally.

    EXPECT_EQ(kit.link_cmd(nullptr), nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE);

    // write reload

    EXPECT_EQ(kit.write(0x00000010, 30), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // read reload

    nsuRegValue_t res;
    EXPECT_EQ(kit.read(0x00000010, &res), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    EXPECT_EQ(kit.set_param("发送指令包头", 0xFF), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);  // set_param reload

    // get_param reload
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
