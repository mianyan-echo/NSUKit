//
// Created by 56585 on 2023/8/18.
//
#include "test_config.h"
#include "NSUKit.h"


TEST(Namespace_nsukit, BaseTest) {
    auto status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    nsukit::NSUSoc<
            nsukit::SimCmdUItf,
            nsukit::SimCmdUItf,
            nsukit::SimStreamUItf,
            nsukit::ICDRegMw,
            nsukit::VirtualChnlMw> kit;

}

