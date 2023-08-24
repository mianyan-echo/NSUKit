//
// Created by 56585 on 2023/8/14.
//

#include "test_config.h"
#include "NSUKit.h"

using namespace nsukit;


class TestCmdUItf: public I_BaseCmdUItf {
public:
    using I_BaseCmdUItf::_fmt_reg_read;
    using I_BaseCmdUItf::_fmt_reg_write;
};


// 注册一个测试，第一个参数为测试集名称，第二个参数为测试名称.
TEST(BaseCmdTest, TestAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}


TEST(BaseCmdTest, FmtRegOverICD) {
    // 创建读取和写入数据的缓冲区
    char read_buffer[20], write_buffer[24];

    // 从基础命令接口获取读取和写入数据包
    auto read_data_packet = TestCmdUItf::_fmt_reg_read(0x32000000);
    auto write_data_packet = TestCmdUItf::_fmt_reg_write(0x32000000, 0x10);

    // 调用接口并将读取的数据存储在 read_buffer 中，检查返回值
    EXPECT_EQ(TestCmdUItf::_fmt_reg_read(0x32000000, read_buffer), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
    // 调用接口并将写入的数据存储在 write_buffer 中，检查返回值
    EXPECT_EQ(TestCmdUItf::_fmt_reg_write(0x32000000, 0x10, write_buffer), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);

    // 期望数据包的大小与预期相符
    EXPECT_EQ(read_data_packet.size(), 20 * sizeof(char));
    EXPECT_EQ(write_data_packet.size(), 24 * sizeof(char));

    // 验证读取缓冲区中的数据与读取数据包中的数据是否一致
    for (int i = 0; i < read_data_packet.size(); i++) {
        EXPECT_EQ(read_buffer[i], read_data_packet[i]);
    }

    // 验证写入缓冲区中的数据与写入数据包中的数据是否一致
    for (int i = 0; i < write_data_packet.size(); i++) {
        EXPECT_EQ(write_buffer[i], write_data_packet[i]);
    }
}


/**
 * 测试
 */
TEST(BaseCmdTest, PCIEInstance) {
    I_BaseCmdUItf *itf = new PCIECmdUItf();
    dynamic_cast<PCIECmdUItf *>(itf)->fake_mode = true;
    EXPECT_EQ(itf->write(0x33000000, 0x10), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
    delete itf;
}
