//
// Created by 56585 on 2023/8/16.
//

#ifndef NSUKIT_TCP_INTERFACE_H
#define NSUKIT_TCP_INTERFACE_H

#include "base_itf.h"
#include "base/c_socket.h"


namespace nsukit {
    class NSU_DLLEXPORT TCPCmdUItf : public I_BaseCmdUItf {
    protected:
        SocketGenerator sockGen;

        float tcpTimeout = 0.;
        std::string ipAddr = "127.0.0.1";
        uint16_t tcpPort = 5001;

        std::mutex opLock;

    public:
        TCPCmdUItf();

        ~TCPCmdUItf() override = default;

        /**
         *
         * @return 是否连接成功
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
         * @param addr
         * @param value
         * @return
         */
        nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) override;

        /**
         *
         * @param addr
         * @param buf
         * @return
         */
        nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t *buf) override;

        /**
         *
         * @param bytes
         * @return
         */
        nsukitStatus_t send_bytes(nsuBytes_t &bytes) override;

        /**
         *
         * @param bytes
         * @return
         */
        nsukitStatus_t send_bytes(nsuCharBuf_p bytes, nsuSize_t length) override;

        /**
         *
         * @param size
         * @param buf
         * @return
         */
        nsukitStatus_t recv_bytes(nsuSize_t size, nsuCharBuf_p buf) override;
    };


    class NSU_DLLEXPORT TCPStreamUItf : public I_BaseStreamUItf {
    public:
        TCPStreamUItf() = default;

        ~TCPStreamUItf() override = default;
    };
}

#endif //NSUKIT_TCP_INTERFACE_H
