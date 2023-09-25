//
// Created by 56585 on 2023/8/17.
//

#ifndef NSUKIT_SIM_INTERFACE_H
#define NSUKIT_SIM_INTERFACE_H

#include "base_itf.h"


namespace nsukit {
    class NSU_DLLEXPORT SimCmdUItf : public I_BaseCmdUItf {
    protected:
        int sim_target = 0;
        int simTimeout = 0;

    public:
        SimCmdUItf();

        ~SimCmdUItf() override = default;

        nsukitStatus_t accept(nsuInitParam_t *param) override;

        nsukitStatus_t close() override;

        nsukitStatus_t set_timeout(float s) override;

        nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) override;

        nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t *buf) override;

        nsukitStatus_t send_bytes(nsuBytes_t &bytes) override;

        nsukitStatus_t send_bytes(nsuCharBuf_p bytes, nsuSize_t length) override;

        nsukitStatus_t recv_bytes(nsuSize_t size, nsuCharBuf_p buf) override;
    };


    class NSU_DLLEXPORT SimStreamUItf : public I_BaseStreamUItf {
    protected:
        struct DMAMemory {
            nsuStreamLen_t length = 0;
            bool self_malloc = false;
            nsuVoidBuf_p buffer = nullptr;
        };
        int sim_target = 0;
        void (*sim_stream_func)(nsuCharBuf_p buf, nsuSize_t length);

        std::map<nsuMemory_p, DMAMemory *> fdMap;
    public:
        SimStreamUItf();

        ~SimStreamUItf() override = default;

        /**
         *
         * @param param
         * @return
         */
        nsukitStatus_t accept(nsuInitParam_t *param) override;

        /**
         *
         * @return
         */
        nsukitStatus_t close() override;

        /**
         *
         * @param s
         * @return
         */
        nsukitStatus_t set_timeout(float s) override;

        /**
         *
         * @param length
         * @param buf
         * @return
         */
        nsuMemory_p alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf = nullptr) override;

        /**
         *
         * @param fd
         * @return
         */
        nsukitStatus_t free_buffer(nsuMemory_p fd) override;

        /**
         *
         * @param fd
         * @param length
         * @return
         */
        nsuVoidBuf_p get_buffer(nsuMemory_p fd, nsuStreamLen_t length) override;

        /**
         *
         * @param chnl
         * @param fd
         * @param length
         * @param offset
         * @return
         */
        nsukitStatus_t
        open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        /**
         *
         * @param chnl
         * @param fd
         * @param length
         * @param offset
         * @return
         */
        nsukitStatus_t
        open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        /**
         *
         * @param fd
         * @param timeout
         * @return
         */
        nsukitStatus_t wait_stream(nsuMemory_p fd, float timeout = 1.) override;

        /**
         *
         * @param fd
         * @return
         */
        nsukitStatus_t break_stream(nsuMemory_p fd) override;

        /**
         *
         * @param chnl
         * @param fd
         * @param length
         * @param offset
         * @param stop_event
         * @param timeout
         * @param flag
         * @return
         */
        nsukitStatus_t
        stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1) override;

        /**
         *
         * @param chnl
         * @param fd
         * @param length
         * @param offset
         * @param stop_event
         * @param timeout
         * @param flag
         * @return
         */
        nsukitStatus_t
        stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1) override;
    };
}

#endif //NSUKIT_SIM_INTERFACE_H
