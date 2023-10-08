//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_ICD_PARSER_H
#define NSUKIT_ICD_PARSER_H


#include <fstream>
#include <cassert>
#include "base_mw.h"
#include "json/json.h"


namespace nsukit {
#define ICD_Uint8 "uint8"
#define ICD_Int8 "int8"
#define ICD_Uint16 "uint16"
#define ICD_Int16 "int16"
#define ICD_Uint32 "uint32"
#define ICD_Int32 "int32"
#define ICD_Float "float"
#define ICD_Double "double"
#define ICD_File "file"
#define ICD_ParamFlag_File "__file__"
#define ICD_ParamFlag_FileLength "__filelength__"


    static std::map<std::string, int> TypeSizeMap = {
            {ICD_Uint8,  1},
            {ICD_Int8,   1},
            {ICD_Uint16, 2},
            {ICD_Int16,  2},
            {ICD_Uint32, 4},
            {ICD_Int32,  4},
            {ICD_Float,  4},
            {ICD_Double, 8}
    };


    /**
     * \note
     * 由一个char指针与一个uint32类型的整数组成
     * 用于描述icd指令中的任意一段数据
     * 可通过socket发送或写入文件
     */
    struct CommandPack {
        char *bytes = nullptr;
        nsuSize_t length = 0;
        nsuSize_t recvLen = 0;
    };

    NSU_DLLEXPORT void DeleteCommandPack(CommandPack *pack);

    NSU_DLLEXPORT uint32_t GetFileSize(const std::string &file_path);

    NSU_DLLEXPORT bool StringEndWith(std::string_view str, std::string_view tail);

    NSU_DLLEXPORT bool StringStartWith(std::string_view str, std::string_view head);


    class NSU_DLLEXPORT ICDRegMw : public I_BaseRegMw {
    protected:
        I_BaseCmdUItf *cs_itf;
        Json::Reader ICDReader;  //icd.json读取器
        Json::Value icdRoot{};
        Json::Value *icdParams{};
        Json::Value *icdCommands{};
        std::string icdPath{};
        bool checkFeedback = false;

        template<class T>
        CommandPack *FmtRegister(const Json::Value &reg, const T &value);

        CommandPack *FmtRegister(const Json::Value &reg, const Json::Value &value);

        CommandPack *FmtRegister(const Json::Value &reg, const std::string &value);

        CommandPack *FmtFile(const std::string &file_path, bool need_size = false);

        nsukitStatus_t EnableParam(nsuCSName_t cname, nsuCharBuf_p buf);

        nsukitStatus_t set_param_by_tname(nsuCSParam_t pname, std::string type, nsuCharBuf_p buf);

    public:

        explicit ICDRegMw(I_BaseCmdUItf *itf);

        ~ICDRegMw();

        nsukitStatus_t config(nsuInitParam_t *param) override;

        bool InitICD(std::string_view path);

        CommandPack *FmtCommand(const std::string &cmd_name, const std::string &file_path = "");

        uint8_t get_param(nsuCSParam_t &param_name, uint8_t _default=0);
        int8_t get_param(nsuCSParam_t &param_name, int8_t _default=0);
        uint16_t get_param(nsuCSParam_t &param_name, uint16_t _default=0);
        int16_t get_param(nsuCSParam_t &param_name, int16_t _default=0);
        uint32_t get_param(nsuCSParam_t &param_name, uint32_t _default=0);
        int32_t get_param(nsuCSParam_t &param_name, int32_t _default=0);
        double get_param(nsuCSParam_t &param_name, double _default=0.);
        float get_param(nsuCSParam_t &param_name, float _default=0.);
        std::string get_param(nsuCSParam_t &param_name, std::string _default="");

        nsukitStatus_t set_param(nsuCSParam_t &param_name, const uint8_t &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const int8_t &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const uint16_t &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const int16_t &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const uint32_t &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const int32_t &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const double &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const float &value) override;
        nsukitStatus_t set_param(nsuCSParam_t &param_name, const std::string &value) override;

        nsukitStatus_t execute(nsuCSName_t cname) override;

        /**
         * 修改某个参数的值
         *
         * @tparam T
         * @param param_name
         * @param value
         * @return
         */
        template<class T>
        nsukitStatus_t set_icd_param(nsuCSParam_t &param_name, const T &value);


        /**
         * 获取参数值
         *
         * @tparam T
         * @param param_name
         * @return
         */
        template<typename T>
        T get_icd_param(nsuCSParam_t &param_name);
    };


    /**
     * 涵盖基础类型，将基础类型值解析为CommandPack
     * @tparam T
     * @param reg
     * @param value
     * @return
     */
    template<class T>
    CommandPack *ICDRegMw::FmtRegister(const Json::Value &reg, const T &value) {
        auto *res = new CommandPack;
        int size;
        size = TypeSizeMap[reg[0].asString()];
        res->bytes = new char[size + 1]();
        res->length = size;
        memcpy(res->bytes, &value, size);
        return res;
    }


    template<class T>
    nsukitStatus_t ICDRegMw::set_icd_param(nsuCSParam_t &param_name, const T &value) {
        Json::Value *param;
        if (!icdParams->isMember(param_name)) return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        param = &(*icdParams)[param_name];
        (*param)[1] = Json::Value(value);
        return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    }


    template<class T>
    T ICDRegMw::get_icd_param(nsuCSParam_t &param_name) {
        Json::Value reg, value;
        T _value;
        if (!icdParams->isMember(param_name)) throw std::runtime_error("此参数不存在");
        reg = (*icdParams)[param_name];
        value = reg[1];

        if (value.isString() && StringStartWith(value.asString(), "0x")) {
            _value = (T) std::stoll(value.asString(), nullptr, 16);
        } else if (value.isString() && StringStartWith(value.asString(), "0b")) {
            _value = (T) std::stoll(value.asString().substr(2), nullptr, 2);
        } else if (value.isString() && reg[0].asString() == ICD_File && std::is_base_of<T, std::string>::value) {
            _value = get_icd_param<T>(param_name);
        } else if (value.isNumeric()) {
            std::string value_type = reg[0].asString();
            if (value_type == ICD_Float) {
                _value = (T) value.asFloat();
            } else if (value_type == ICD_Double) {
                _value = (T) value.asDouble();
            } else if (value_type == ICD_Uint8) {
                _value = (T) value.asUInt();
            } else if (value_type == ICD_Int8) {
                _value = (T) value.asInt();
            } else if (value_type == ICD_Uint16) {
                _value = (T) value.asUInt();
            } else if (value_type == ICD_Int16) {
                _value = (T) value.asInt();
            } else if (value_type == ICD_Uint32) {
                _value = (T) value.asUInt();
            } else if (value_type == ICD_Int32) {
                _value = (T) value.asInt();
            } else {
                std::cout << "未识别的值类型 " << value.toStyledString() << std::endl;
                throw std::runtime_error("未识别的寄存器值类型");
            }
        } else {
            std::cout << "未识别的值类型 " << value.toStyledString() << std::endl;
            throw std::runtime_error("未识别的寄存器值类型");
        }
        return _value;
    }

    /**
     * 模板特化，针对file类型的参数，或者想以字符串形式表示的参数
     * @param param_name 参数名称
     * @return string
     */
    template<>
    inline std::string ICDRegMw::get_icd_param<std::string>(nsuCSParam_t &param_name) {
        if (!icdParams->isMember(param_name)) return "";
        return (*icdParams)[param_name][1].asString();
    }
}
#endif //NSUKIT_ICD_PARSER_H
