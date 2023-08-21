//
// Created by 56585 on 2023/8/15.
//

#ifndef NSUKIT_BASE_MW_H
#define NSUKIT_BASE_MW_H

#include "tools/config.h"

namespace nsukit {
    class DLLEXPORT U_Middleware {
    public:
        virtual nsukitStatus_t config(nsuAcceptParam_t *param) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }
    };


    class DLLEXPORT I_BaseRegMw : public U_Middleware {
    public:
        explicit I_BaseRegMw() = default;

        explicit I_BaseRegMw(BaseKit *kit) {}

    };


    class DLLEXPORT I_BaseChnlMw : public U_Middleware {
    public:
        explicit I_BaseChnlMw() = default;

        explicit I_BaseChnlMw(BaseKit *kit) {}

    };
}

#endif //NSUKIT_BASE_MW_H
