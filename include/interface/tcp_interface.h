//
// Created by 56585 on 2023/8/16.
//

#ifndef NSUKIT_TCP_INTERFACE_H
#define NSUKIT_TCP_INTERFACE_H

#include "base_itf.h"


namespace nsukit {
    class DLLEXPORT TCPCmdUItf : public I_BaseCmdUItf {
    public:
        TCPCmdUItf() = default;

        ~TCPCmdUItf() override = default;
    };


    class DLLEXPORT TCPStreamUItf : public I_BaseStreamUItf {
    public:
        TCPStreamUItf() = default;

        ~TCPStreamUItf() override = default;
    };
}

#endif //NSUKIT_TCP_INTERFACE_H
