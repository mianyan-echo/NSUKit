#ifndef NSUKIT_BASE_KIT_H
#define NSUKIT_BASE_KIT_H

#include "utils/config.h"
#include "interface/interface.h"
#include "middleware/middleware.h"


namespace nsukit {
    class NSU_DLLEXPORT BaseKit {
    public:
        virtual nsukitStatus_t link_cmd(nsuInitParam_t *param) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t unlink_cmd() { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t link_stream(nsuInitParam_t *param) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t unlink_stream() { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t bulk_write(
                nsuRegAddr_t base, nsuCharBuf_p values, nsuBulkMode mode=nsuBulkMode::INCREMENT) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t bulk_read(
                nsuRegAddr_t base, nsuSize_t length,
                nsuRegValue_t *buf = nullptr, nsuBulkMode mode=nsuBulkMode::INCREMENT) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t set_param(nsuICDParam_t addr, nsuRegValue_t value, bool execute) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t set_param(nsuICDParam_t addr, nsuICDParam_t value, bool execute) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t set_param(nsuICDParam_t addr, nsuRegValue_t *buf) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t set_param(nsuICDParam_t addr, nsuICDParam_t *buf) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t execute(nsuICDParam_t name) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsuMemory_p alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf = nullptr) { return nullptr; }

        virtual nsukitStatus_t free_buffer(nsuMemory_p fd) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsuVoidBuf_p get_buffer(nsuMemory_p fd, nsuStreamLen_t length) { return nullptr; }

        virtual nsukitStatus_t
        stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, int flag = 1) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, int flag = 1) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        wait_stream(nsuMemory_p fd, time_t timeout = 0) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t break_stream(nsuMemory_p fd) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }
    };
}

#endif //NSUKIT_BASE_KIT_H
