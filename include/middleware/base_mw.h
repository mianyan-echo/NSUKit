//
// Created by 56585 on 2023/8/15.
//

#ifndef NSUKIT_BASE_MW_H
#define NSUKIT_BASE_MW_H

#include "base/base_kit.h"
#include "interface/base_itf.h"

namespace nsukit {
    class NSU_DLLEXPORT U_Middleware {
    public:
        virtual nsukitStatus_t config(nsuInitParam_t *param) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }
    };


    class NSU_DLLEXPORT I_BaseRegMw : public U_Middleware {
    public:
        explicit I_BaseRegMw() = default;

        explicit I_BaseRegMw(I_BaseCmdUItf *kit) {}

        ~I_BaseRegMw() = default;

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

        virtual nsukitStatus_t execute(nsuCSName_t cname);
    };


    class NSU_DLLEXPORT I_BaseStreamMw : public U_Middleware {
    public:
        explicit I_BaseStreamMw() = default;

        explicit I_BaseStreamMw(I_BaseStreamUItf *itf) {};

        virtual nsukitStatus_t
        open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0);

        virtual nsukitStatus_t
        open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0);

        virtual nsukitStatus_t wait_stream(nsuMemory_p fd, float timeout = 1.);

        virtual nsukitStatus_t break_stream(nsuMemory_p fd);

        virtual nsukitStatus_t
        stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1);

        virtual nsukitStatus_t
        stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1);

    };
}

#endif //NSUKIT_BASE_MW_H
