//
// Created by 56585 on 2023/8/15.
//

#ifndef NSUKIT_PCIE_INTERFACE_H
#define NSUKIT_PCIE_INTERFACE_H

#include "base_itf.h"
#include "xdma_api.h"


namespace nsukit {
    class XDMAOperation_t {
    protected:
        std::map<int, int> openedBoard = {};
        std::mutex OpLock{};

    public:
        /**
         * 开启板卡，并记录开启次数，只有第一次真正开启板卡
         * @param board xdma板卡号
         * @return
         */
        nsukitStatus_t open_xdma_board(int board);

        /**
         * 关闭板卡并记录关闭次数，只有最后一次真正关闭
         * @param board xdma板卡号
         * @return
         */
        nsukitStatus_t close_xdma_board(int board);
    };


    class NSU_DLLEXPORT PCIECmdUItf : public I_BaseCmdUItf {
    protected:
        nsuBoardNum_t pciBoard = 0;
        nsuRegAddr_t sentBase = 0;
        nsuRegAddr_t recvBase = 0;
        int pciTimeout = 0;

    public:
        /**
         * @note 将基类 nsukit::I_BaseCmdUItf 的成员变量mixin_赋值为 nsukit::Mixin_NativeRegCmd 的实例
         */
        PCIECmdUItf() { mixin_ = new Mixin_NativeRegCmd(this); };

        ~PCIECmdUItf() override { delete mixin_; };

        /**
         *
         * @param board 板卡号
         * @param sent_base ICDOverAXI发送基地址
         * @param recv_base ICDOverAXI接收基地址
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


    class NSU_DLLEXPORT PCIEStreamUItf : public I_BaseStreamUItf {
    private:
        const uint8_t byteWidth = 4;
        const uint8_t maxChnl = 4;
    protected:
        struct StreamProcess {
            nsuStreamLen_t current = 0;
            nsuStreamLen_t total = 0;
        };

        nsuBoardNum_t pciBoard = 0;

        std::map<nsuMemory_p, StreamProcess> uploadProcess{};

        std::map<nsuMemory_p, StreamProcess> downloadProcess{};
    public:
        /**
         *
         */
        PCIEStreamUItf();

        ~PCIEStreamUItf() override = default;

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
#endif //NSUKIT_PCIE_INTERFACE_H
