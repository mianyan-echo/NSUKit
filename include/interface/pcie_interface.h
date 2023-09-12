//
// Created by 56585 on 2023/8/15.
//

#ifndef NSUKIT_PCIE_INTERFACE_H
#define NSUKIT_PCIE_INTERFACE_H

#include "base_itf.h"


namespace nsukit {
    class DLLEXPORT PCIECmdUItf : public I_BaseCmdUItf {
    protected:
        nsuBoardNum_t pciBoard = 0;
        nsuRegAddr_t sentBase = 0;
        nsuRegAddr_t recvBase = 0;
        int pciTimeout = 0;

    public:
        bool fake_mode = false;

        PCIECmdUItf();

        ~PCIECmdUItf() override = default;

        nsukitStatus_t accept(nsuAcceptParam_t *param) override;

        nsukitStatus_t close() override;

        nsukitStatus_t set_timeout(int s) override;

        nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) override;

        nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t *buf) override;

        nsukitStatus_t send_bytes(nsuBytes_t &bytes) override;

        nsukitStatus_t send_bytes(nsuCharBuf_p bytes) override;

        nsukitStatus_t recv_bytes(nsuSize_t size, nsuCharBuf_p buf) override;
    };


    class DLLEXPORT PCIEStreamUItf : public I_BaseStreamUItf {
    public:
        PCIEStreamUItf();

        ~PCIEStreamUItf() override = default;

        nsukitStatus_t open_board() override;

        nsukitStatus_t close() override;
    };
}
#endif //NSUKIT_PCIE_INTERFACE_H
