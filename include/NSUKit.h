//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_NSUKIT_H
#define NSUKIT_NSUKIT_H

#include "base/base_kit.h"
#include "version.h"
#include "interface/interface.h"
#include "middleware/middleware.h"
#include "high_level/SuperAWG.h"


// 锁定nsukit三参数方法中的第三个参数的默认值
#define LOCK_NSUKIT_METHOD_3P(name, t1, t2, value) nsukitStatus_t name(t1 a, t2 b) {return name(a, b, value);}

#define METHOD_NEED_(func) if (!func()) { return nsukitStatus_t::NSUKIT_STATUS_TEMP_MISMATCH; }


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
        bool cmd_linked = false;
        bool stream_linked = false;

    protected:
        CmdMw_t *mw_cmd;
        ChnlMw_t *mw_chnl;
        CSItf_t *itf_cs;
        CRItf_t *itf_cr;
        DSItf_t *itf_ds;

        bool check_typesafe() {
//            return itf_cs_typesafe && itf_cr_typesafe && itf_ds_typesafe && mw_stream_typesafe && mw_cmd_typesafe;
            return true;
        }

        bool combined_cmd_itf() {return (I_BaseCmdUItf *)itf_cs == (I_BaseCmdUItf *)itf_cr;}

    public:
        NSUSoc();
        ~NSUSoc();

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
        nsukitStatus_t link_cmd(nsuInitParam_t *param) override;

        /**
         *
         * @return
         */
        nsukitStatus_t unlink_cmd() override;

        /**
         * 开启数据流之前准备
         * @details 根据目标设备类型、参数建立链接
         * @param param
         * @return
         */
        nsukitStatus_t link_stream(nsuInitParam_t *param) override;

        /**
         *
         * @return
         */
        nsukitStatus_t unlink_stream() override;

        /**
         * 写寄存器
         * @details 按照输入的地址、值进行写寄存器
         * @param addr 写入寄存器地址，u32
         * @param value 写入寄存器值，32b
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
         * @param base 块写入的基地址
         * @param values 要写入的一串值，char*
         * @param length 要写入的数据长度
         * @param mode - 块写入模式
         * - nsuBulkMode::INCREMENT  从基地址开始递增写入数据
         * - nsuBulkMode::LOOP  将数据依次写入单个基地址
         * @return nsukitStatus_t
         */
        nsukitStatus_t
        bulk_write(nsuRegAddr_t base, nsuCharBuf_p values,
                   nsuSize_t length, nsuBulkMode mode=nsuBulkMode::INCREMENT) override;

        /**
         * 块读取
         * @param base 块读取的基地址
         * @param length 要读取的数据长度
         * @param buf 读取缓存
         * @param mode 块读取入模式
         * @return
         */
        nsukitStatus_t
        bulk_read(nsuRegAddr_t base, nsuSize_t length,
                  nsuVoidBuf_p buf = nullptr, nsuBulkMode mode=nsuBulkMode::INCREMENT) override;

        /**
         *
         * @param name
         * @param value
         * @return
         */
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const uint8_t &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const int8_t &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const uint16_t &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const int16_t &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const uint32_t &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const int32_t &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const double &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const float &value) override;
        nsukitStatus_t
        set_param(nsuCSParam_t &param_name, const std::string &value) override;

        /**
         *
         * @param name
         * @param value
         * @return
         */
        uint8_t
        get_param(nsuCSParam_t &param_name, uint8_t _default = 0) override;
        int8_t
        get_param(nsuCSParam_t &param_name, int8_t _default = 0) override;
        uint16_t
        get_param(nsuCSParam_t &param_name, uint16_t _default = 0) override;
        int16_t
        get_param(nsuCSParam_t &param_name, int16_t _default = 0) override;
        uint32_t
        get_param(nsuCSParam_t &param_name, uint32_t _default = 0) override;
        int32_t
        get_param(nsuCSParam_t &param_name, int32_t _default = 0) override;
        double
        get_param(nsuCSParam_t &param_name, double _default = 0.) override;
        float
        get_param(nsuCSParam_t &param_name, float _default = 0.) override;
        std::string
        get_param(nsuCSParam_t &param_name, std::string _default = "") override;

        /**
         *
         * @param name
         * @param value
         * @return
         */
        nsukitStatus_t
        execute(nsuCSParam_t cname) override;

        /**
         * 申请一片数据流用的内存
         * @param length 要申请的内存长度 单位Bytes
         * @param buf 外部填入一片申请好的内存首地址指针，可为null，不为null时，代表指定一片内存，作为数据流专用内存
         * @return 数据流内存标识
         */
        nsuMemory_p
        alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf = nullptr) override;

        /**
         * 根据内存标识释放对应的内存
         * @param fd nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @return
         */
        nsukitStatus_t
        free_buffer(nsuMemory_p fd) override;

        /**
         * 获取数据流内存标识的真实内存首指针
         * @param fd 数据流内存标识，nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @param length 所需数据流内存长度
         * @return
         */
        nsuVoidBuf_p
        get_buffer(nsuMemory_p fd, nsuStreamLen_t length=0) override;

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

        /**
         * 开启一次阻塞式数据流下行
         * @param chnl 数据流通道
         * @param fd nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @param length 数据流下行数据量，单位Byte
         * @param offset 从fd中对应的偏移量下行
         * @param stop_event 函数指针，停止event
         * @return
         */
        nsukitStatus_t
        stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, int flag = 1) override;

        /**
         * 异步开启一次数据流下行
         * @param chnl 数据流通道
         * @param fd nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @param length 数据流下行数据量，单位Byte
         * @param offset 从fd中对应的偏移量下行
         * @return
         */
        nsukitStatus_t
        open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        /**
         * 异步开启一次数据流上行
         * @param chnl 数据流通道
         * @param fd nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @param length 数据流上行数据量，单位Byte
         * @param offset 数据流上行到fd中对应的偏移量
         * @return
         */
        nsukitStatus_t
        open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) override;

        /**
         * 等待fd对应的数据流交互完成
         * @param fd nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @param timeout
         * @return
         */
        nsukitStatus_t
        wait_stream(nsuMemory_p fd, float timeout = 0.) override;

        /**
         * 终止一次数据流交互
         * @param fd nsukit::NSUSoc::alloc_buffer申请到的内存标识
         * @return
         */
        nsukitStatus_t
        break_stream(nsuMemory_p fd) override;
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
            delete itf_cr;
        }
        delete itf_cs;
        delete itf_ds;
        delete mw_cmd;
        delete mw_chnl;
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::link_cmd(nsuInitParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        METHOD_NEED_(check_typesafe);

        //
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        if (!this->combined_cmd_itf()) {
            auto* crInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
            status |= crInterface->accept(param);
        }
        //
        auto* csInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cs);
        status |= csInterface->accept(param);
        //
        auto* cmdMiddleware = dynamic_cast<I_BaseRegMw*>(mw_cmd);
        status |= cmdMiddleware->config(param);

        if (status == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            cmd_linked = true;
        }

        //
        return status;
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::unlink_cmd() {
        METHOD_NEED_(check_typesafe);
        //
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        if (!this->combined_cmd_itf()) {
            auto* crInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
            status |= crInterface->close();
        }
        status |= dynamic_cast<I_BaseCmdUItf *>(itf_cs)->close();
        if (status == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            cmd_linked = false;
        }
        return status;
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::link_stream(nsuInitParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        METHOD_NEED_(check_typesafe);

        //
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        //
        auto* chnlInterface = dynamic_cast<I_BaseStreamUItf*>(itf_ds);
        status |= chnlInterface->accept(param);
        //
        auto* chnlMiddleware = dynamic_cast<I_BaseStreamMw*>(mw_chnl);
        status |= chnlMiddleware->config(param);

        if (status == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            stream_linked = true;
        }

        //
        return status;
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::unlink_stream() {
        METHOD_NEED_(check_typesafe);
        auto status = dynamic_cast<I_BaseStreamUItf *>(itf_ds)->close();
        if (status == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            stream_linked = false;
        }
        return status;
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
        if (!cmd_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->execute(cname);
    }

    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    uint8_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, uint8_t _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    int8_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, int8_t _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    uint16_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, uint16_t _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    int16_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, int16_t _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    uint32_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, uint32_t _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    int32_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, int32_t _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    double
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, double _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    float
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, float _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    std::string
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::get_param(nsuCSParam_t &param_name, std::string _default) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->get_param(param_name, _default);
    }

    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const uint8_t &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const int8_t &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const uint16_t &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const int16_t &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const uint32_t &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const int32_t &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const double &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const float &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
    }
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::set_param(nsuCSParam_t &param_name, const std::string &value) {
        auto mw = dynamic_cast<I_BaseRegMw *>(mw_cmd);
        return mw->set_param(param_name, value);
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
        if (!stream_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
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
        if (!stream_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->stream_recv(chnl, fd, length, offset, stop_event, 0, flag);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd,
                                                                                     nsuStreamLen_t length,
                                                                                     nsuStreamLen_t offset,
                                                                                     bool (*stop_event)(), int flag) {
        METHOD_NEED_(check_typesafe);
        if (!stream_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->stream_send(chnl, fd, length, offset, stop_event, 0, flag);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::open_send(nsuChnlNum_t chnl, nsuMemory_p fd,
                                                                                   nsuStreamLen_t length,
                                                                                   nsuStreamLen_t offset) {
        METHOD_NEED_(check_typesafe);
        if (!stream_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->open_send(chnl, fd, length, offset);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd,
                                                                                   nsuStreamLen_t length,
                                                                                   nsuStreamLen_t offset) {
        METHOD_NEED_(check_typesafe);
        if (!stream_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->open_recv(chnl, fd, length, offset);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::wait_stream(nsuMemory_p fd, float timeout) {
        METHOD_NEED_(check_typesafe);
        if (!stream_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->wait_stream(fd, timeout);
    }


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::break_stream(nsuMemory_p fd) {
        METHOD_NEED_(check_typesafe);
        if (!stream_linked) {
            return nsukitStatus_t::NSUKIT_STATUS_NOT_LINK;
        }
        auto mw = dynamic_cast<I_BaseStreamMw *>(mw_chnl);
        return mw->break_stream(fd);
    }

}
#endif //NSUKIT_NSUKIT_H
