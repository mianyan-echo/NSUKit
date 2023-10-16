//
// Created by 56585 on 2023/8/16.
//

#ifndef NSUKIT_VIRTUAL_CHNL_H
#define NSUKIT_VIRTUAL_CHNL_H

#include "base_mw.h"


namespace nsukit {
    class NSU_DLLEXPORT VirtualChnlMw : public I_BaseStreamMw, public RegOperationMixin {
    protected:
        using StreamMode = nsuInitParam_t::StreamMode;

        static uint16_t VCHNL_NUM;
        static uint16_t R2V_CHNL;
        static nsuRegAddr_t PARAM_ADDR;
        static nsuRegAddr_t PARAM_WR_ADDR;
        static nsuRegAddr_t STATUS_ADDR;

        I_BaseStreamUItf *ds_itf;
        StreamMode streamMode;


    public:
        explicit VirtualChnlMw(I_BaseStreamUItf *itf);

        ~VirtualChnlMw() = default;

        nsukitStatus_t config(nsuInitParam_t *param) override;

        nsukitStatus_t
        open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        nsukitStatus_t
        open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        nsukitStatus_t wait_stream(nsuMemory_p fd, float timeout = 1.) override;

        nsukitStatus_t break_stream(nsuMemory_p fd) override;

        nsukitStatus_t
        stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1) override;

        nsukitStatus_t
        stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1) override;
    };
}

#endif //NSUKIT_VIRTUAL_CHNL_H
