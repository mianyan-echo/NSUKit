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

        ~TCPCmdUItf() override;

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
    protected:
        struct Memory {
            nsuMemory_p memory;
            bool self_alloc = true;
            std::string error_msg{};
            nsuStreamLen_t mem_size;
            nsuStreamLen_t using_size;
            nsuStreamLen_t need_size;
            unsigned int idx;
            ThreadSafeEvent finish_event;
            std::mutex mtx;
        };

        SocketGenerator sockGen{5002, "127.0.0.1", false};

        float tcpTimeout = 0.;
        std::string ipAddr = "127.0.0.1";
        uint16_t tcpPort = 5002;
        uint32_t block_size = 4 * 1024 * 1024;

        ThreadSafeEvent stop_event;
        std::mutex opLock;
        std::thread upload_thread;
        std::thread download_thread;

        std::map<unsigned int, Memory *> memory_dict{};
        unsigned int memory_index = 0;

        static void
        _recv(SocketGenerator *sock, ThreadSafeEvent *stop_event, Memory *mem, uint32_t block, nsuStreamLen_t length,
              nsuStreamLen_t offset = 0);

    public:
        TCPStreamUItf() = default;

        ~TCPStreamUItf() override;

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
                    bool(*stop_event)() = nullptr, float timeout = 5., int flag = 1) override;

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
                    bool(*stop_event)() = nullptr, float timeout = 5., int flag = 1) override;
    };
}

#endif //NSUKIT_TCP_INTERFACE_H
