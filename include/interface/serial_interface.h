//
// Created by 56585 on 2023/8/21.
//

#ifndef NSUKIT_SERIAL_INTERFACE_H
#define NSUKIT_SERIAL_INTERFACE_H

#include "base_itf.h"


namespace nsukit {
    class DLLEXPORT SerialCmdUItf: public I_BaseCmdUItf {
    public:
        SerialCmdUItf() = default;

        ~SerialCmdUItf() override = default;
    };

    class DLLEXPORT SerialChnlUItf: public I_BaseChnlUItf {
    public:
        SerialChnlUItf() = default;

        ~SerialChnlUItf() override = default;

    };
}

#endif //NSUKIT_SERIAL_INTERFACE_H
