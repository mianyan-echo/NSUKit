//
// Created by 56585 on 2023/12/23.
//

#ifndef NSUKIT_SUPERAWG_H
#define NSUKIT_SUPERAWG_H

#include "base/base_kit.h"

namespace nsukit {
    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT hl_sawg_init(nsukit::BaseKit *kit);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    hl_sawg_send_wave(nsukit::BaseKit *kit, nsuRegAddr_t addr, nsuStreamLen_t length);
}

#endif //NSUKIT_SUPERAWG_H
