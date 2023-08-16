//
// Created by 56585 on 2023/8/16.
//

#ifndef NSUKIT_VIRTUAL_CHNL_H
#define NSUKIT_VIRTUAL_CHNL_H

#include "base_mw.h"


class VirtualChnlMw: public I_BaseChnlMw{
public:
    explicit VirtualChnlMw(BaseKit *kit) {}
    ~VirtualChnlMw() = default;
};

#endif //NSUKIT_VIRTUAL_CHNL_H
