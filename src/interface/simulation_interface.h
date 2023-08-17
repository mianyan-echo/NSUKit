//
// Created by 56585 on 2023/8/17.
//

#ifndef NSUKIT_SIMULATION_INTERFACE_H
#define NSUKIT_SIMULATION_INTERFACE_H

#include "base_itf.h"


class SimCmdUItf: public I_BaseCmdUItf {
protected:
    int sim_param_a = 0;
    int sim_param_b = 0;
    int simTimeout = 0;

public:
    SimCmdUItf();
    ~SimCmdUItf() override = default;

    nsukitStatus_t accept(nsuAcceptParam_t *param) override;

    nsukitStatus_t close() override;

    nsukitStatus_t set_timeout(int s) override;

    nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) override;

    nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t* buf) override;

    nsukitStatus_t send_bytes(nsuBytes_t &bytes) override;

    nsukitStatus_t send_bytes(nsuCharBuf_p bytes) override;

    nsukitStatus_t recv_bytes(nsuSize_t size, nsuCharBuf_p buf) override;
};


class SimChnlUItf: public I_BaseChnlUItf {
public:
    SimChnlUItf();
    ~SimChnlUItf() override = default;

    nsukitStatus_t open_board() override;

    nsukitStatus_t close() override;
};

#endif //NSUKIT_SIMULATION_INTERFACE_H
