//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_NSUKIT_H
#define NSUKIT_NSUKIT_H

#include "base/base_kit.h"
#include "version.h"
#include "interface/interface.h"
#include "middleware/middleware.h"


// 锁定nsukit三参数方法中的第三个参数的默认值
#define LOCK_NSUKIT_METHOD_3P(name, t1, t2, value) nsukitStatus_t name(t1 a, t2 b) {return name(a, b, value);}

#define METHOD_NEED_(func) if (!func()) return nsukitStatus_t::NSUKIT_STATUS_TEMP_MISMATCH


namespace nsukit {
    /**
     *
     * @tparam CSItf_t nsukit::I_BaseCmdUItf 的子类
     * @tparam CRItf_t nsukit::I_BaseCmdUItf 的子类
     * @tparam DSItf_t nsukit::I_BaseStreamUItf 的子类
     * @tparam CmdMw_t nsukit::I_BaseRegMw 的子类
     * @tparam ChnlMw_t nsukit::I_BaseStreamMw 的子类
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t=ICDRegMw, class ChnlMw_t=VirtualChnlMw>
    class NSUSoc : public BaseKit {
    private:
        bool itf_ds_typesafe = true;
        bool itf_cs_typesafe = true;
        bool itf_cr_typesafe = true;
        bool mw_stream_typesafe = true;
        bool mw_cmd_typesafe = true;

    protected:
        CmdMw_t *mw_cmd;
        ChnlMw_t *mw_chnl;
        CSItf_t *itf_cs;
        CRItf_t *itf_cr;
        DSItf_t *itf_ds;

        bool check_typesafe() {
            return itf_cs_typesafe and itf_cr_typesafe and itf_ds_typesafe and mw_stream_typesafe and mw_cmd_typesafe;
        }

        bool combined_cmd_itf() {return (I_BaseCmdUItf *)itf_cs == (I_BaseCmdUItf *)itf_cr;}

    public:
        NSUSoc();
        ~NSUSoc();

        nsukitStatus_t link_cmd(nsuInitParam_t *param) override;

        nsukitStatus_t unlink_cmd() override;

        nsukitStatus_t link_stream(nsuInitParam_t *param) override;

        nsukitStatus_t unlink_stream() override;

        /**
         * 写寄存器
         * @details 按照输入的地址、值进行写寄存器
         * @param addr 写入寄存器地址
         * @param value 写入寄存器值
         * @return
         */
        nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) override;

        /**
         * 读寄存器
         * @param addr 读取寄存器地址
         * @param buf  要接收读取值的buffer
         * @return
         */
        nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t* buf) override;

        /**
         * 块写入
         * @param base
         * @param values
         * @param mode
         * @return
         */
        nsukitStatus_t
        bulk_write(nsuRegAddr_t base, nsuCharBuf_p values,
                   nsuSize_t length, nsuBulkMode mode=nsuBulkMode::INCREMENT) override;

        /**
         * 块读取
         * @param base
         * @param length
         * @param buf
         * @param mode
         * @return
         */
        nsukitStatus_t
        bulk_read(nsuRegAddr_t base, nsuSize_t length,
                  nsuVoidBuf_p buf = nullptr, nsuBulkMode mode=nsuBulkMode::INCREMENT) override;

        /**
         *
         * @tparam T
         * @param name
         * @param value
         * @return
         */
        template<typename T>
        nsukitStatus_t set_param(nsuCSParam_t name, T value);

        template<typename T>
        T get_param(nsuCSParam_t name, T _default=0);

        std::string get_param(nsuCSParam_t &param_name, std::string _default="") override;

        nsukitStatus_t execute(nsuCSParam_t cname) override;

        /**
         * 申请一片数据流用的内存
         * @param length 要申请的内存长度
         * @param buf 外部填入一片申请好的内存首地址指针
         * @return 数据流内存标识
         */
        nsuMemory_p alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf = nullptr) override;

        /**
         * 释放数据流用的内存
         * @param fd 数据流内存标识
         * @return
         */
        nsukitStatus_t free_buffer(nsuMemory_p fd) override;

        /**
         * 获取数据流内存标识的真实内存首指针
         * @param fd 数据流内存标识
         * @param length 所需数据流内存长度
         * @return
         */
        nsuVoidBuf_p get_buffer(nsuMemory_p fd, nsuStreamLen_t length) override;

        /**
         * 阻塞式开启一次数据流上行
         * @param chnl 数据流通道号
         * @param fd nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @param length 要上行的数据长度
         * @param offset 上行数据到内存的首地址偏移值
         * @param stop_event 停止函数
         * @param flag 预留
         * @return
         */
        nsukitStatus_t
        stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, int flag = 1) override;

        nsukitStatus_t
        stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, int flag = 1) override;

        nsukitStatus_t
        open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        nsukitStatus_t
        open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        nsukitStatus_t
        wait_stream(nsuMemory_p fd, float timeout = 0.) override;

        nsukitStatus_t break_stream(nsuMemory_p fd) override;
    };


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::NSUSoc() {
        itf_cs_typesafe = std::is_base_of<I_BaseCmdUItf, CSItf_t>::value;
        itf_cr_typesafe = std::is_base_of<I_BaseCmdUItf, CRItf_t>::value;
        itf_ds_typesafe = std::is_base_of<I_BaseStreamUItf, DSItf_t>::value;
        mw_cmd_typesafe = std::is_base_of<I_BaseRegMw, CmdMw_t>::value;
        mw_stream_typesafe = std::is_base_of<I_BaseStreamMw, ChnlMw_t>::value;

        itf_cs = new CSItf_t();
        if (std::is_same<CSItf_t, CRItf_t>::value) {
            itf_cr = (CRItf_t *)itf_cs;
        } else {
            itf_cr = new CRItf_t();
        }
        itf_ds = new DSItf_t();
        mw_cmd = new CmdMw_t(itf_cs);
        mw_chnl = new ChnlMw_t(itf_ds);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::~NSUSoc() {
        if (!this->combined_cmd_itf()) {
            free(itf_cr);
        }
        free(itf_cs);
        free(itf_ds);
        free(mw_cmd);
        free(mw_chnl);
    }

    /**
     * 发送指令前准备
     * @details 根据目标设备类型、参数建立链接
     *
     * @code
     * #include "NSUKit.h"
     *
     * auto kit = NSUKit::NSUSoc<PCIECmdUItf, PCIEStreamUItf>();
     * nsukitStatus_t status;
     * nsuXDMAParam_t param {};
     * param.board = 0;
     * status = kit.link_cmd(&param);
     * status = kit.write(0x00000035, 15);
     * @endcode
     *
     * @param param
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::link_cmd(nsuInitParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        METHOD_NEED_(check_typesafe);

        // 初始化状态为成功
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        if (!this->combined_cmd_itf()) {
            auto* crInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
            status |= crInterface->accept(param);
        }
        // 调用命令接口的 accept 函数，将结果合并到状态中
        auto* csInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cs);
        status |= csInterface->accept(param);
        // 调用命令中间件的 config 函数，将结果合并到状态中
        auto* cmdMiddleware = dynamic_cast<I_BaseRegMw*>(mw_cmd);
        status |= cmdMiddleware->config(param);

        // 返回合并后的状态
        return status;
    }


    /**
     *
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::unlink_cmd() {
        METHOD_NEED_(check_typesafe);
        // 初始化状态为成功
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        if (!this->combined_cmd_itf()) {
            auto* crInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
            status |= crInterface->close();
        }
        return dynamic_cast<I_BaseCmdUItf *>(itf_cs)->close();
    }


    /**
     * 开启数据流之前准备
     * @details 根据目标设备类型、参数建立链接
     * @param param
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::link_stream(nsuInitParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        METHOD_NEED_(check_typesafe);

        // 初始化状态为成功
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        // 调用命令接口的 accept 函数，将结果合并到状态中
        auto* chnlInterface = dynamic_cast<I_BaseStreamUItf*>(itf_ds);
        status |= chnlInterface->accept(param);
        // 调用命令中间件的 config 函数，将结果合并到状态中
        auto* chnlMiddleware = dynamic_cast<I_BaseStreamMw*>(mw_chnl);
        status |= chnlMiddleware->config(param);

        // 返回合并后的状态
        return status;
    }


    /**
     *
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::unlink_stream() {
        METHOD_NEED_(check_typesafe);
        return dynamic_cast<I_BaseStreamUItf *>(itf_ds)->close();
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::write(nsuRegAddr_t addr, nsuRegValue_t value) {
        METHOD_NEED_(check_typesafe);

        auto cmdInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
        return cmdInterface->write(addr, value);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
        METHOD_NEED_(check_typesafe);

        auto status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
        auto* cmdInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
        status |= cmdInterface->read(addr, buf);
        return status;
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::bulk_write(nsuRegAddr_t base, nsuCharBuf_p values,
                                                                     nsuSize_t length, nsuBulkMode mode) {
        auto cmdInterface = dynamic_cast<I_BaseCmdUItf *>(itf_cr);
        if (mode == nsuBulkMode::INCREMENT) {
            return cmdInterface->increment_write(base, values, length);
        }
        if (mode == nsuBulkMode::LOOP) {
            return cmdInterface->loop_write(base, values, length);
        }
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::bulk_read(nsuRegAddr_t base, nsuSize_t length,
                                                                                   nsuVoidBuf_p buf, nsuBulkMode mode) {
        auto cmdInterface = dynamic_cast<I_BaseCmdUItf *>(itf_cr);
        if (mode == nsuBulkMode::INCREMENT) {
            return cmdInterface->increment_read(base, length, buf);
        }
        if (mode == nsuBulkMode::LOOP) {
            return cmdInterface->loop_read(base, length, buf);
        }
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::execute(nsuCSParam_t cname) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->execute(cname);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    template<typename T>
    T NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t name, T _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(name, _default);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    std::string
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, std::string _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    template<typename T>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t name, T value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(name, value);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsuMemory_p
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf) {
        if (!check_typesafe()) {
            return nullptr;
        }
        auto itf = dynamic_cast<I_BaseStreamUItf *>(itf_ds);
        return itf->alloc_buffer(length, buf);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::free_buffer(nsuMemory_p fd) {
        METHOD_NEED_(check_typesafe);
        auto itf = dynamic_cast<I_BaseStreamUItf *>(itf_ds);
        return itf->free_buffer(fd);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsuVoidBuf_p
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_buffer(nsuMemory_p fd, nsuStreamLen_t length) {
        if (!check_typesafe()) {
            return nullptr;
        }
        auto itf = dynamic_cast<I_BaseStreamUItf *>(itf_ds);
        return itf->get_buffer(fd, length);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd,
                                                                                     nsuStreamLen_t length,
                                                                                     nsuStreamLen_t offset,
                                                                                     bool(*stop_event) (), int flag) {
        METHOD_NEED_(check_typesafe);
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->stream_recv(chnl, fd, length, offset, stop_event, 0, flag);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd,
                                                                                     nsuStreamLen_t length,
                                                                                     nsuStreamLen_t offset,
                                                                                     bool (*stop_event)(), int flag) {
        METHOD_NEED_(check_typesafe);
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->stream_send(chnl, fd, length, offset, stop_event, 0, flag);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::open_send(nsuChnlNum_t chnl, nsuMemory_p fd,
                                                                                   nsuStreamLen_t length,
                                                                                   nsuStreamLen_t offset) {
        METHOD_NEED_(check_typesafe);
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->open_send(chnl, fd, length, offset);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd,
                                                                                   nsuStreamLen_t length,
                                                                                   nsuStreamLen_t offset) {
        METHOD_NEED_(check_typesafe);
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->open_recv(chnl, fd, length, offset);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::wait_stream(nsuMemory_p fd, float timeout) {
        METHOD_NEED_(check_typesafe);
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->wait_stream(fd, timeout);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::break_stream(nsuMemory_p fd) {
        METHOD_NEED_(check_typesafe);
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->break_stream(fd);
    }

}
#endif //NSUKIT_NSUKIT_H
