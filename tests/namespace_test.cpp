//
// Created by 56585 on 2023/8/18.
//
#define NSUKIT_IN_TEST

#include "test_config.h"
#include "NSUKit.h"


TEST(Namespace_nsukit, BaseTest) {
    auto status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    nsukit::NSUKit<
            nsukit::PCIECmdUItf,
            nsukit::PCIEChnlUItf,
            nsukit::ICDRegMw,
            nsukit::VirtualChnlMw> kit;

}

