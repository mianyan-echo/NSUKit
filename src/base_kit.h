#ifndef NSUKIT_BASE_KIT_H
#define NSUKIT_BASE_KIT_H

#include "tools/config.h"
#include "interface/interface.h"
#include "middleware/middleware.h"


class BaseKit {
public:
    virtual nsukitStatus_t start_command(nsuAcceptParam_t *param=nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsukitStatus_t stop_command() {return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;}

    virtual nsukitStatus_t start_stream(nsuAcceptParam_t *param=nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsukitStatus_t stop_stream() {return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;}

    template<typename T>
    nsukitStatus_t write(nsuRegAddr_t addr, T value, bool execute=true) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    template<typename T>
    nsukitStatus_t write(nsuICDParam_t addr, T value, bool execute=true) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t * buf= nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsukitStatus_t read(nsuICDParam_t addr, nsuRegValue_t * buf= nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    template<typename T>
    nsukitStatus_t bulk_write(std::initializer_list<nsuRegAddr_t> addrs, std::initializer_list<T> values) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsukitStatus_t bulk_read(std::initializer_list<nsuRegAddr_t> list, std::vector<nsuRegValue_t> *buf= nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsuMemory_p alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf= nullptr) {return nullptr;}

    virtual nsukitStatus_t free_buffer(nsuMemory_p fd) {return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;}

    virtual nsuVoidBuf_p get_buffer(nsuMemory_p fd, nsuStreamLen_t length) {return nullptr;}

    virtual nsukitStatus_t stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset=0,
                                       void * stop_event=nullptr, int flag=1) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsukitStatus_t stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset=0,
                                       void * stop_event=nullptr, int flag=1) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }

    virtual nsuStreamLen_t send_open(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset=0) {
        return 0;
    }

    virtual nsuStreamLen_t recv_open(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset=0) {
        return 0;
    }

    virtual nsuStreamLen_t wait_dma(nsuMemory_p fd, time_t timeout=0) {
        return 0;
    }

    virtual nsukitStatus_t break_dma(nsuMemory_p fd) {
        return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
    }
};

#endif //NSUKIT_BASE_KIT_H
