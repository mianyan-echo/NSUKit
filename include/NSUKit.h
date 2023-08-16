//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_NSUKIT_H
#define NSUKIT_NSUKIT_H

#ifdef linux

#define _API_CALL
#define DLLEXPORT extern "C"
#else  //win, rtx

#define _API_CALL __stdcall
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

#ifdef NSUKIT_IN_TEST
#include "base_kit.h"
#endif

namespace NSUKit {
    /**
     *
     * @tparam CmdItf_t I_Base
     * @tparam ChnlItf_t
     * @tparam CmdMw_t
     * @tparam ChnlMw_t
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t=ICDRegMw, class ChnlMw_t=VirtualChnlMw>
    class NSUKit : public BaseKit {
    protected:
        CmdMw_t *mw_cmd;
        ChnlMw_t *mw_chnl;
        CmdItf_t *itf_cmd;
        ChnlItf_t *itf_chnl;

    public:
        NSUKit();
        ~NSUKit();

        nsukitStatus_t start_command(nsuAcceptParam_t *param) override;

        virtual nsukitStatus_t stop_command() {return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;}

        virtual nsukitStatus_t start_stream(nsuAcceptParam_t *param=nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t stop_stream() {return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;}
    };


    /**
     *
     * @tparam CmdItf_t
     * @tparam ChnlItf_t
     * @tparam CmdMw_t
     * @tparam ChnlMw_t
     * @param param
     * @return
     */
    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    nsukitStatus_t NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::start_command(nsuAcceptParam_t *param) {
        if (param == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
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


    template<class CmdItf_t, class ChnlItf_t, class CmdMw_t, class ChnlMw_t>
    NSUKit<CmdItf_t, ChnlItf_t, CmdMw_t, ChnlMw_t>::NSUKit() {
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
}
#endif //NSUKIT_NSUKIT_H
