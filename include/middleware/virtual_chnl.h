//
// Created by 56585 on 2023/8/16.
//

#ifndef NSUKIT_VIRTUAL_CHNL_H
#define NSUKIT_VIRTUAL_CHNL_H

#include "base_mw.h"
#include "interface/base_itf.h"


namespace nsukit {
    class NSU_DLLEXPORT VirtualChnlMw : public I_BaseStreamMw, public RegOperationMixin {
    public:
        explicit VirtualChnlMw(BaseKit *kit) {}

        ~VirtualChnlMw() = default;

        nsukitStatus_t config(nsuInitParam_t *param) override;
    };
}

#endif //NSUKIT_VIRTUAL_CHNL_H
