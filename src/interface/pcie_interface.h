//
// Created by 56585 on 2023/8/15.
//

#ifndef NSUKIT_PCIE_INTERFACE_H
#define NSUKIT_PCIE_INTERFACE_H

#include "base_itf.h"


class PCIECmdUItf: public I_BaseCmdUItf {
protected:
    nsuBoardNum_t pciBoard=0;
    nsuRegAddr_t sentBase=0;
    nsuRegAddr_t recvBase=0;
    int pciTimeout=0;

public:
    bool fake_mode= false;

    PCIECmdUItf();
    ~PCIECmdUItf() override = default;

    nsukitStatus_t accept(nsuAcceptParam_t *param) override;

    nsukitStatus_t close() override;

    nsukitStatus_t set_timeout(int s) override;

    nsukitStatus_t write(std::any addr, std::any value) override;

    std::any read(std::any addr) override;

    nsukitStatus_t send_bytes() override;

    nsukitStatus_t recv_bytes() override;
};


class PCIEChnlUItf: public I_BaseChnlUItf {
public:
    PCIEChnlUItf();
    ~PCIEChnlUItf() override = default;
};

#endif //NSUKIT_PCIE_INTERFACE_H
