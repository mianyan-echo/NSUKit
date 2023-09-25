//
// Created by 56585 on 2023/8/15.
//

#ifndef NSUKIT_BASE_MW_H
#define NSUKIT_BASE_MW_H

#include "../utils/config.h"

namespace nsukit {
    class NSU_DLLEXPORT U_Middleware {
    public:
        virtual nsukitStatus_t config(nsuInitParam_t *param) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }
    };


    class NSU_DLLEXPORT I_BaseRegMw : public U_Middleware {
    public:
        explicit I_BaseRegMw() = default;

        explicit I_BaseRegMw(BaseKit *kit) {}

    };


    class NSU_DLLEXPORT I_BaseStreamMw : public U_Middleware {
    public:
        explicit I_BaseStreamMw() = default;

        explicit I_BaseStreamMw(BaseKit *kit) {}

    };
}

#endif //NSUKIT_BASE_MW_H
