#ifndef NSUKIT_BASE_KIT_H
#define NSUKIT_BASE_KIT_H

#include "config.h"


namespace nsukit {
    DLLEXTERN class NSU_DLLEXPORT BaseKit {
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
                nsuRegAddr_t base, nsuCharBuf_p values, nsuSize_t length, nsuBulkMode mode=nsuBulkMode::INCREMENT) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t bulk_read(
                nsuRegAddr_t base, nsuSize_t length,
                nsuVoidBuf_p buf = nullptr, nsuBulkMode mode=nsuBulkMode::INCREMENT) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual uint8_t get_param(nsuCSParam_t &param_name, uint8_t _default=0);
        virtual int8_t get_param(nsuCSParam_t &param_name, int8_t _default=0);
        virtual uint16_t get_param(nsuCSParam_t &param_name, uint16_t _default=0);
        virtual int16_t get_param(nsuCSParam_t &param_name, int16_t _default=0);
        virtual uint32_t get_param(nsuCSParam_t &param_name, uint32_t _default=0);
        virtual int32_t get_param(nsuCSParam_t &param_name, int32_t _default=0);
        virtual double get_param(nsuCSParam_t &param_name, double _default=0.);
        virtual float get_param(nsuCSParam_t &param_name, float _default=0.);
        virtual std::string get_param(nsuCSParam_t &param_name, std::string _default="");


        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const uint8_t &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const int8_t &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const uint16_t &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const int16_t &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const uint32_t &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const int32_t &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const double &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const float &value);
        virtual nsukitStatus_t set_param(nsuCSParam_t &param_name, const std::string &value);

        virtual nsukitStatus_t execute(nsuCSParam_t cname);

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
        wait_stream(nsuMemory_p fd, float timeout = 0.) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t break_stream(nsuMemory_p fd) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }
    };
}

#endif //NSUKIT_BASE_KIT_H
