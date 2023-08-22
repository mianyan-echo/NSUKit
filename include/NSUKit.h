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
     * @tparam CmdItf_t I_Base
     * @tparam ChnlItf_t
     * @tparam CmdMw_t
     * @tparam ChnlMw_t
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t=ICDRegMw, class ChnlMw_t=VirtualChnlMw>
    class NSUKit : public BaseKit {
    private:
        bool itf_chnl_typesafe = true;
        bool itf_cmd_typesafe = true;
        bool mw_chnl_typesafe = true;
        bool mw_cmd_typesafe = true;

    protected:
        CmdMw_t *mw_cmd;
        ChnlMw_t *mw_chnl;
        CmdItf_t *itf_cmd;
        ChnlItf_t *itf_chnl;

        bool check_typesafe() {return itf_cmd_typesafe and itf_chnl_typesafe and mw_chnl_typesafe and mw_cmd_typesafe;}

    public:
        NSUKit();
        ~NSUKit();

        nsukitStatus_t start_command(nsuAcceptParam_t *param) override;

        nsukitStatus_t stop_command() override;

        nsukitStatus_t start_stream(nsuAcceptParam_t *param) override;

        nsukitStatus_t stop_stream() override;

        nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value, bool execute) override;
        LOCK_NSUKIT_METHOD_3P(write, nsuRegAddr_t, nsuRegValue_t, true)

        nsukitStatus_t write(nsuICDParam_t addr, nsuRegValue_t value, bool execute) override;
        LOCK_NSUKIT_METHOD_3P(write, nsuICDParam_t, nsuRegValue_t, true)

        nsukitStatus_t write(nsuICDParam_t addr, nsuICDParam_t value, bool execute) override;
        LOCK_NSUKIT_METHOD_3P(write, nsuICDParam_t, nsuICDParam_t, true)

        nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t* buf) override;

        nsukitStatus_t read(nsuICDParam_t addr, nsuRegValue_t* buf) override;

        nsukitStatus_t read(nsuICDParam_t addr, nsuICDParam_t* buf) override;
    };


    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::NSUKit() {
        itf_cmd_typesafe = std::is_base_of<I_BaseCmdUItf, CmdItf_t>::value;
        itf_chnl_typesafe = std::is_base_of<I_BaseChnlUItf, ChnlItf_t>::value;
        mw_cmd_typesafe = std::is_base_of<I_BaseRegMw, CmdMw_t>::value;
        mw_chnl_typesafe = std::is_base_of<I_BaseChnlMw, ChnlMw_t>::value;

        itf_cmd = new CmdItf_t();
        itf_chnl = new ChnlItf_t();
        mw_cmd = new CmdMw_t(this);
        mw_chnl = new ChnlMw_t(this);
    }


    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::~NSUKit() {
        free(itf_cmd);
        free(itf_chnl);
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
     * auto kit = NSUKit::NSUKit<PCIECmdUItf, PCIEChnlUItf>();
     * nsukitStatus_t status;
     * nsuXDMAParam_t param {};
     * param.board = 0;
     * status = kit.start_command(&param);
     * status = kit.write(0x00000035, 15);
     * @endcode
     *
     * @param param
     * @return
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::start_command(nsuAcceptParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        METHOD_NEED_(check_typesafe);

        // 初始化状态为成功
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        // 调用命令接口的 accept 函数，将结果合并到状态中
        auto* cmdInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cmd);
        status |= cmdInterface->accept(param);
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
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::stop_command() {
        METHOD_NEED_(check_typesafe);
        return dynamic_cast<I_BaseCmdUItf *>(itf_cmd)->close();
    }


    /**
     * 开启数据流之前准备
     * @details 根据目标设备类型、参数建立链接
     * @param param
     * @return
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::start_stream(nsuAcceptParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        METHOD_NEED_(check_typesafe);

        // 初始化状态为成功
        nsukitStatus_t status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

        // 调用命令接口的 accept 函数，将结果合并到状态中
        auto* chnlInterface = dynamic_cast<I_BaseChnlUItf*>(itf_chnl);
        status |= chnlInterface->accept(param);
        status |= chnlInterface->open_board();
        // 调用命令中间件的 config 函数，将结果合并到状态中
        auto* chnlMiddleware = dynamic_cast<I_BaseChnlMw*>(mw_chnl);
        status |= chnlMiddleware->config(param);

        // 返回合并后的状态
        return status;
    }


    /**
     *
     * @return
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::stop_stream() {
        METHOD_NEED_(check_typesafe);
        return dynamic_cast<I_BaseChnlUItf *>(itf_chnl)->close();
    }


    /**
     * 写寄存器
     * @details 按照输入的地址、值进行写寄存器
     * @param addr
     * @param value
     * @param execute
     * @return
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::write(nsuRegAddr_t addr, nsuRegValue_t value, bool execute) {
        METHOD_NEED_(check_typesafe);

        auto status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
        if (execute) {
            auto* cmdInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cmd);
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
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::write(nsuICDParam_t addr, nsuRegValue_t value, bool execute) {
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
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t
    NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::write(nsuICDParam_t addr, nsuICDParam_t value, bool execute) {
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
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
        METHOD_NEED_(check_typesafe);

        auto status = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
        auto* cmdInterface = dynamic_cast<I_BaseCmdUItf*>(itf_cmd);
        status |= cmdInterface->read(addr, buf);
        return status;
    }


    /**
     *
     * @param addr
     * @param buf
     * @return
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::read(nsuICDParam_t addr, nsuRegValue_t *buf) {
        METHOD_NEED_(check_typesafe);

        return BaseKit::read(addr, buf);
    }


    /**
     *
     * @param addr
     * @param buf
     * @return
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::read(nsuICDParam_t addr, nsuICDParam_t *buf) {
        METHOD_NEED_(check_typesafe);

        return BaseKit::read(addr, buf);
    }

}
#endif //NSUKIT_NSUKIT_H
