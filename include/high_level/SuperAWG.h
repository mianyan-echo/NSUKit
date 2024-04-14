/// @todo SuperAWG封装待实现

#ifndef NSUKIT_SUPERAWG_H
#define NSUKIT_SUPERAWG_H

#include "base/base_kit.h"

namespace nsukit {
    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    /**
     * @warning 未实现
     * @param kit
     * @return
     */
    hl_sawg_init(nsukit::BaseKit *kit);

    DLLEXTERN nsukitStatus_t NSU_DLLEXPORT
    /**
     * @warning 未实现
     * @param kit
     * @param addr
     * @param length
     * @return
     */
    hl_sawg_send_wave(nsukit::BaseKit *kit, nsuRegAddr_t addr, nsuStreamLen_t length);
}

#endif //NSUKIT_SUPERAWG_H
