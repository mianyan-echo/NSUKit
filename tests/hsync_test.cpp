//
// Created by 56585 on 2024/1/2.
//
#include "test_config.h"
#include "NSUKit.h"
#include <future>


int add(int a, int b) {
    return a + b;
}


//TEST(HLTEST, HAS_ASYNC) {
//    int num_parallel = 4;
//    nsukit::NSUSoc<nsukit::SimCmdUItf, nsukit::SimCmdUItf, nsukit::SimStreamUItf> soc{};
//    nsuInitParam_t param;
//    soc.link_cmd(&param);
//    nsukitStatus_t res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
//    // async库实现
//    std::future<nsukitStatus_t> futures[num_parallel];
//    for(int i=0; i < num_parallel; i++) {
//        futures[i] = std::async(std::launch::async, &nsukit::BaseKit::execute, &soc, "系统停止");
//        // [&soc](){return soc.execute("系统停止");}
//    }
//    for(int i=0; i < num_parallel; i++) {
//        res |= futures[i].get();
//    }
//
//    std::cout << nsukit::status2_string(res) << std::endl;
//    EXPECT_EQ(res, nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
//}

#ifdef TEST_HARDWARE_SYNC
#include "high_level/HardwareSync.h"

typedef nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::TCPCmdUItf, nsukit::SimStreamUItf> SocType;

TEST(HLTEST, HSYNC_SYNC) {
    int argc = 3;

    char *argv[3] = {"dsfsd", "192.168.2.12", "192.168.2.12"};

    SocType master{};
    nsukit::BaseKit* slaves[argc-2];
    nsuInitParam_t master_param;
    nsuInitParam_t slaves_param[argc-2];

    master_param.cmd_ip = argv[1];
    auto res = master.link_cmd(&master_param);
    for(int i=2;i<argc;i++) {
        auto s_kit = new SocType;
        slaves_param[i-2].cmd_ip = argv[i];
        res |= s_kit->link_cmd(&(slaves_param[i-2]));
        slaves[i-2] = s_kit;
    }

    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立连接：" << nsukit::status2_string(res) << std::endl;
    }

    char cmd_fmt[] = "hsync_s1\nhsync_s2\nhsync_s3\nhsync_s4\nhsync_s5\nhsync_s6\n";
    res |= nsukit::hl_hsync_sync(&master, slaves, argc-2, cmd_fmt);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "多板同步：" << nsukit::status2_string(res) << std::endl;
    }
}

#endif