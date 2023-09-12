//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_NSUKIT_H
#define NSUKIT_NSUKIT_H

#include "base_kit.h"
#include "version.h"

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
        bool mw_chnl_typesafe = true;
        bool mw_cmd_typesafe = true;

    protected:
        CmdMw_t *mw_cmd;
        ChnlMw_t *mw_chnl;
        CSItf_t *itf_cs;
        CRItf_t *itf_cr;
        DSItf_t *itf_ds;

        bool check_typesafe() {
            return itf_cs_typesafe and itf_cr_typesafe and itf_ds_typesafe and mw_chnl_typesafe and mw_cmd_typesafe;
        }

        bool combined_cmd_itf() {return itf_cs == itf_cr;}

    public:
        NSUSoc();
        ~NSUSoc();

        nsukitStatus_t link_cmd(nsuAcceptParam_t *param) override;

        nsukitStatus_t unlink_cmd() override;

        nsukitStatus_t link_stream(nsuAcceptParam_t *param) override;

        nsukitStatus_t unlink_stream() override;

        nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value, bool execute) override;

        nsukitStatus_t write(nsuICDParam_t addr, nsuRegValue_t value, bool execute) override;

        nsukitStatus_t write(nsuICDParam_t addr, nsuICDParam_t value, bool execute) override;

        nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t* buf) override;

        nsukitStatus_t read(nsuICDParam_t addr, nsuRegValue_t* buf) override;

        nsukitStatus_t read(nsuICDParam_t addr, nsuICDParam_t* buf) override;
    };


    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::NSUSoc() {
        itf_cs_typesafe = std::is_base_of<I_BaseCmdUItf, CSItf_t>::value;
        itf_cr_typesafe = std::is_base_of<I_BaseCmdUItf, CRItf_t>::value;
        itf_ds_typesafe = std::is_base_of<I_BaseStreamUItf, DSItf_t>::value;
        mw_cmd_typesafe = std::is_base_of<I_BaseRegMw, CmdMw_t>::value;
        mw_chnl_typesafe = std::is_base_of<I_BaseStreamMw, ChnlMw_t>::value;

        itf_cs = new CSItf_t();
        if (std::is_same<CSItf_t, CRItf_t>::value) {
            itf_cr = itf_cs;
        } else {
            itf_cr = new CRItf_t();
        }
        itf_ds = new DSItf_t();
        mw_cmd = new CmdMw_t(this);
        mw_chnl = new ChnlMw_t(this);
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
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::link_cmd(nsuAcceptParam_t *param) {
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
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::link_stream(nsuAcceptParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        METHOD_NEED_(check_typesafe);

        // 初始化状态为成功
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        // 调用命令接口的 accept 函数，将结果合并到状态中
        auto* chnlInterface = dynamic_cast<I_BaseStreamUItf*>(itf_ds);
        status |= chnlInterface->accept(param);
        status |= chnlInterface->open_board();
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


    /**
     * 写寄存器
     * @details 按照输入的地址、值进行写寄存器
     * @param addr
     * @param value
     * @param execute
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::write(nsuRegAddr_t addr, nsuRegValue_t value, bool execute) {
        METHOD_NEED_(check_typesafe);

        auto status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
        if (execute) {
            auto* cmdInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
            status |= cmdInterface->write(addr, value);
        }
        return status;
    }


    /**
     * 写ICD参数
     * @details 按照输入的ICD参数名称、值进行写入到参数中
     * @param addr icd参数名
     * @param value uint32 值
     * @param execute 是否执行对应的ICD指令
     * @return 接口执行状态
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::write(nsuICDParam_t addr, nsuRegValue_t value, bool execute) {
        METHOD_NEED_(check_typesafe);

        // TODO: 补充实现
        return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    }


    /**
     * 写ICD参数
     * @details 按照输入的ICD参数名称、值进行写入到参数中
     * @param addr icd参数名
     * @param value 字符串值
     * @param execute 是否执行对应的ICD指令
     * @return 接口执行状态
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::write(nsuICDParam_t addr, nsuICDParam_t value, bool execute) {
        METHOD_NEED_(check_typesafe);

        // TODO: 补充实现
        return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    }


    /**
     * 读寄存器
     * @param addr
     * @param buf
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
        METHOD_NEED_(check_typesafe);

        auto status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
        auto* cmdInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cr);
        status |= cmdInterface->read(addr, buf);
        return status;
    }


    /**
     *
     * @param addr
     * @param buf
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::read(nsuICDParam_t addr, nsuRegValue_t *buf) {
        METHOD_NEED_(check_typesafe);

        return BaseKit::read(addr, buf);
    }


    /**
     *
     * @param addr
     * @param buf
     * @return
     */
    template<class CSItf_t, class CRItf_t, class DSItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUSoc<CSItf_t, CRItf_t, DSItf_t, CmdMw_t, ChnlMw_t>::read(nsuICDParam_t addr, nsuICDParam_t *buf) {
        METHOD_NEED_(check_typesafe);

        return BaseKit::read(addr, buf);
    }

}
#endif //NSUKIT_NSUKIT_H
