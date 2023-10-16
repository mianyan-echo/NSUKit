//
// Created by 56585 on 2023/8/14.
//

#include "test_config.h"
#include "NSUKit.h"

using namespace nsukit;


class TestCase_UseCmdMixin: public nsukit::I_BaseCmdUItf {
protected:
    std::vector<std::map<nsuRegAddr_t, nsuRegValue_t> > *reg_map;
public:
    TestCase_UseCmdMixin(std::vector<std::map<nsuRegAddr_t, nsuRegValue_t> > *map) {
        reg_map = map;
        mixin_ = new nsukit::Mixin_NativeRegCmd(this);};

    ~TestCase_UseCmdMixin() override {delete mixin_;};

    nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) override {
        std::map<nsuRegAddr_t, nsuRegValue_t> _m {{addr, value}};
        reg_map->push_back(_m);
        return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    }
};


// 注册一个测试，第一个参数为测试集名称，第二个参数为测试名称.
TEST(BaseCmdTest, TestAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);

    auto _t = std::chrono::milliseconds(0);

    auto a = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(_t);
    auto b = std::chrono::steady_clock::now();
    std::cout << (b-a).count() << std::endl;
}


//TEST(BaseCmdTest, FmtRegOverICD) {
//    // 创建读取和写入数据的缓冲区
//    char read_buffer[20], write_buffer[24];
//
//    // 从基础命令接口获取读取和写入数据包
//    auto read_data_packet = TestCmdUItf::_fmt_reg_read(0x32000000);
//    auto write_data_packet = TestCmdUItf::_fmt_reg_write(0x32000000, 0x10);
//
//    // 调用接口并将读取的数据存储在 read_buffer 中，检查返回值
//    EXPECT_EQ(TestCmdUItf::_fmt_reg_read(0x32000000, read_buffer), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
//    // 调用接口并将写入的数据存储在 write_buffer 中，检查返回值
//    EXPECT_EQ(TestCmdUItf::_fmt_reg_write(0x32000000, 0x10, write_buffer), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
//
//    // 期望数据包的大小与预期相符
//    EXPECT_EQ(read_data_packet.size(), 20 * sizeof(char));
//    EXPECT_EQ(write_data_packet.size(), 24 * sizeof(char));
//
//    // 验证读取缓冲区中的数据与读取数据包中的数据是否一致
//    for (int i = 0; i < read_data_packet.size(); i++) {
//        EXPECT_EQ(read_buffer[i], read_data_packet[i]);
//    }
//
//    // 验证写入缓冲区中的数据与写入数据包中的数据是否一致
//    for (int i = 0; i < write_data_packet.size(); i++) {
//        EXPECT_EQ(write_buffer[i], write_data_packet[i]);
//    }
//}


/**
 * 测试
 */
TEST(BaseCmdTest, PCIEInstance) {
    I_BaseCmdUItf *itf = new SimCmdUItf();
    EXPECT_EQ(itf->write(0x33000000, 0x10), nsukitStatus_t::NSUKIT_STATUS_SUCCESS);
    delete itf;
}


/**
 * 测试nsukit::mixin_NativeRegCmd中的方法是否正确被执行
 */
TEST(CmdMixinTest, NativeRegCmd) {
    std::vector<std::map<nsuRegAddr_t, nsuRegValue_t> > map;
    TestCase_UseCmdMixin itf{&map};
    uint8_t vdata[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    nsuRegValue_t vres[2] = {0, 0};
    memcpy(vres, vdata, 5);

    itf.increment_write(0x00, vdata, 5);
    EXPECT_EQ(map[0][0x00], vres[0]);
    EXPECT_EQ(map[1][0x04], vres[1]);

    itf.loop_write(0x100, vdata, 5);
    EXPECT_EQ(map[2][0x100], vres[0]);
    EXPECT_EQ(map[3][0x100], vres[1]);
}


/**
 *
 */
TEST(CmdMixinTest, VirtualRegCmd) {
}
