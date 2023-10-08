//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_BASE_ITF_H
#define NSUKIT_BASE_ITF_H

#include "base/base_kit.h"


namespace nsukit {
    /**
     * @class RegOperationMixin
     */
    class NSU_DLLEXPORT RegOperationMixin {
    public:
        virtual nsukitStatus_t reg_write(nsuRegAddr_t addr, nsuRegValue_t value) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t reg_read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }
    };


    class NSU_DLLEXPORT U_Interface {
        friend class RegOperationMixin;

    public:
        virtual ~U_Interface() = default;

        virtual nsukitStatus_t accept(nsuInitParam_t *param) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t close() { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t set_timeout(float s) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }
    };


    class NSU_DLLEXPORT U_BaseCmdMixin {
    public:
        virtual nsukitStatus_t multi_write(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t> &value) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t multi_read(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t *> &value) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        increment_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        increment_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        loop_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        loop_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }
    };


    class NSU_DLLEXPORT I_BaseCmdUItf: public U_Interface {
    public:
        U_BaseCmdMixin *mixin_ = nullptr;

        virtual nsukitStatus_t send_bytes(nsuBytes_t &bytes) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t send_bytes(nsuCharBuf_p bytes, nsuSize_t length) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t
        recv_bytes(nsuSize_t size, nsuCharBuf_p buf) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t write(nsuRegAddr_t addr, nsuRegValue_t value) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        nsukitStatus_t multi_write(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t> &value) {
            if (mixin_== nullptr) {
                return nsukitStatus_t::NSUKIT_STATUS_MISMATCH_MIXIN;
            }
            return mixin_->multi_write(addr, value);
        }

        nsukitStatus_t multi_read(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t *> &value) {
            if (mixin_== nullptr) {
                return nsukitStatus_t::NSUKIT_STATUS_MISMATCH_MIXIN;
            }
            return mixin_->multi_read(addr, value);
        }

        nsukitStatus_t
        increment_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            if (mixin_== nullptr) {
                return nsukitStatus_t::NSUKIT_STATUS_MISMATCH_MIXIN;
            }
            return mixin_->increment_write(addr, value, length, reg_len);
        }

        nsukitStatus_t
        increment_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            if (mixin_== nullptr) {
                return nsukitStatus_t::NSUKIT_STATUS_MISMATCH_MIXIN;
            }
            return mixin_->increment_read(addr, length, value, reg_len);
        }

        nsukitStatus_t
        loop_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            if (mixin_== nullptr) {
                return nsukitStatus_t::NSUKIT_STATUS_MISMATCH_MIXIN;
            }
            return mixin_->loop_write(addr, value, length, reg_len);
        }

        nsukitStatus_t
        loop_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            if (mixin_== nullptr) {
                return nsukitStatus_t::NSUKIT_STATUS_MISMATCH_MIXIN;
            }
            return mixin_->loop_read(addr, length, value, reg_len);
        }
    };


    class NSU_DLLEXPORT Mixin_NativeRegCmd: public U_BaseCmdMixin {
    private:
        I_BaseCmdUItf *cmd_itf_;
    public:
        explicit Mixin_NativeRegCmd(I_BaseCmdUItf* base) : cmd_itf_(base) {};

        nsukitStatus_t multi_write(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t> &value);

        nsukitStatus_t multi_read(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t *> &value);

        nsukitStatus_t
        increment_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH);

        nsukitStatus_t
        increment_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH);

        nsukitStatus_t
        loop_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH);

        nsukitStatus_t
        loop_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH);
    };


    class NSU_DLLEXPORT Mixin_VirtualRegCmd: public U_BaseCmdMixin {
    private:
        I_BaseCmdUItf *cmd_itf_;
    protected:
        struct RegPack {
            uint32_t head = 0x5F5F5F5F;
            uint32_t id = 0;
            uint32_t num = 0x00000000;
            uint32_t length = 20;
        };

        static nsuBytes_t _fmt_reg_read(nsuRegAddr_t reg);

        template<typename T>
        static nsuBytes_t _fmt_reg_write(nsuRegAddr_t reg, T value);

        static nsukitStatus_t _fmt_reg_read(nsuRegAddr_t reg, nsuCharBuf_p buf);

        template<typename T>
        static nsukitStatus_t _fmt_reg_write(nsuRegAddr_t reg, T value, nsuCharBuf_p buf);

    public:
        explicit Mixin_VirtualRegCmd(I_BaseCmdUItf* base) : cmd_itf_(base) {};

        nsukitStatus_t multi_write(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t> &value) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        nsukitStatus_t multi_read(std::vector<nsuRegAddr_t> &addr, std::vector<nsuRegValue_t *> &value) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        nsukitStatus_t
        increment_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        nsukitStatus_t
        increment_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        nsukitStatus_t
        loop_write(nsuRegAddr_t addr, nsuVoidBuf_p value, nsuSize_t length, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        nsukitStatus_t
        loop_read(nsuRegAddr_t addr, nsuSize_t length, nsuVoidBuf_p value, nsuSize_t reg_len = NSU_REG_BWIDTH) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }
    };


    /**
     * 通道类
     */
    class NSU_DLLEXPORT I_BaseStreamUItf : public U_Interface {
    public:
        virtual nsuMemory_p alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf= nullptr) { return 0; }

        virtual nsukitStatus_t free_buffer(nsuMemory_p fd) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsuVoidBuf_p get_buffer(nsuMemory_p fd, nsuStreamLen_t length) { return nullptr; }

        virtual nsukitStatus_t
        open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t wait_stream(nsuMemory_p fd, float timeout = 1.) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t break_stream(nsuMemory_p fd) { return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD; }

        virtual nsukitStatus_t
        stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }

        virtual nsukitStatus_t
        stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset = 0,
                    bool(*stop_event) () = nullptr, float timeout = 5., int flag = 1) {
            return nsukitStatus_t::NSUKIT_STATUS_NEED_RELOAD;
        }
    };


    /**
     * 模拟写寄存器的ICD指令
     * @tparam T 任意类型值
     * @param reg 寄存器地址
     * @param value 寄存器值，会截取前32bit
     * @param buf 存放ICD指令的缓存
     * @return 是否格式化成功
     */
    template<typename T>
    nsukitStatus_t Mixin_VirtualRegCmd::_fmt_reg_write(nsuRegAddr_t reg, T value, nsuCharBuf_p buf) {
//        static_assert(std::is_integral<T>::value, "T must be an integral type");
        static constexpr uint32_t DataPacketID = 0x31000000;
        static constexpr size_t DataPacketLength = sizeof(RegPack) + sizeof(nsuRegAddr_t) + sizeof(T);

        if (buf == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;  // 缓冲区为空，返回错误
        }

        RegPack pack;
        pack.id = DataPacketID;
        pack.length = DataPacketLength;

        memcpy(buf, &pack, sizeof(RegPack));
        memcpy(buf + sizeof(RegPack), &reg, sizeof(nsuRegAddr_t));
        memcpy(buf + sizeof(RegPack) + sizeof(nsuRegAddr_t), &value, sizeof(T));

        return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;  // 数据格式化成功
    }


    /**
     * 模拟写寄存器的ICD指令
     * @tparam T 任意类型值
     * @param reg 寄存器地址
     * @param value 寄存器值，会截取前32bit
     * @return nsuBytes_t char类型向量
     */
    template<typename T>
    nsuBytes_t Mixin_VirtualRegCmd::_fmt_reg_write(nsuRegAddr_t reg, T value) {
        nsuBytes_t res;
        uint32_t size = sizeof(RegPack) + sizeof(nsuRegAddr_t) + sizeof(T);
        char _res[size];
        RegPack pack;
        pack.id = 0x31000000;
        pack.length = size;
        memcpy(_res, &pack, sizeof(RegPack));
        memcpy(_res + sizeof(RegPack), &reg, sizeof(nsuRegAddr_t));
        memcpy(_res + sizeof(RegPack) + sizeof(nsuRegAddr_t), &value, sizeof(T));

        for (auto ch: _res) {
            res.push_back(ch);
        }
        return res;
    }
}

#endif //NSUKIT_BASE_ITF_H
