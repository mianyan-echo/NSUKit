//
// Created by 56585 on 2023/9/22.
//
#include "test_config.h"
#include "NSUKit.h"

using namespace nsukit;

TEST(DataUploadTest, BasicTest) {
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
