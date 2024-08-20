//
// Created by 56585 on 2023/10/11.
//

#ifndef NSUKIT_JSON_ICD_WRAPPER_H
#define NSUKIT_JSON_ICD_WRAPPER_H

#include "middleware/icd_parser.h"
#include "json/json.h"

namespace nsukit {
    class JsonWrapper {
    public:
        Json::Reader ICDReader;  //icd.json��ȡ��
        Json::Value icdRoot{};
        Json::Value *icdParams{};
        Json::Value *icdCommands{};

        bool InitICD(std::string_view path);

        /**
         * ���ǻ������ͣ�����������ֵ����ΪCommandPack
         * @tparam T
         * @param reg
         * @param value
         * @return
         */
        template<class T>
        CommandPack *FmtRegister(const Json::Value &reg, const T &value);

        CommandPack *FmtRegister(const Json::Value &reg, const Json::Value &value);

        CommandPack *FmtRegister(const Json::Value &reg, const std::string &value);

        CommandPack *FmtFile(const std::string &file_path, bool need_size = false);

        /**
         * �޸�ĳ��������ֵ
         *
         * @tparam T
         * @param param_name
         * @param value
         * @return
         */
        template<class T>
        nsukitStatus_t set_icd_param(nsuCSParam_t &param_name, const T &value);


        /**
         * ��ȡ����ֵ
         *
         * @tparam T
         * @param param_name
         * @return
         */
        template<typename T>
        T get_icd_param(nsuCSParam_t &param_name);
    };


    template<class T>
    CommandPack *JsonWrapper::FmtRegister(const Json::Value &reg, const T &value) {
        auto *res = new CommandPack;
        int size;
        size = TypeSizeMap[reg[0].asString()];
        res->bytes = new char[size + 1]();
        res->length = size;
        memcpy(res->bytes, &value, size);
        return res;
    }


    template<class T>
    nsukitStatus_t JsonWrapper::set_icd_param(nsuCSParam_t &param_name, const T &value) {
        Json::Value *param;
        if (!icdParams->isMember(param_name)) return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        param = &(*icdParams)[param_name];
        (*param)[1] = Json::Value(value);
        return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    }


    template<class T>
    T JsonWrapper::get_icd_param(nsuCSParam_t &param_name) {
        Json::Value reg, value;
        T _value;
        if (!icdParams->isMember(param_name)){
            throw std::runtime_error("�˲���������");
        }
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
                std::cout << "δʶ���ֵ���� " << value.toStyledString() << std::endl;
                throw std::runtime_error("δʶ��ļĴ���ֵ����");
            }
        } else {
            std::cout << "δʶ���ֵ���� " << value.toStyledString() << std::endl;
            throw std::runtime_error("δʶ��ļĴ���ֵ����");
        }
        return _value;
    }

    /**
     * ģ���ػ������file���͵Ĳ��������������ַ�����ʽ��ʾ�Ĳ���
     * @param param_name ��������
     * @return string
     */
    template<>
    inline std::string JsonWrapper::get_icd_param<std::string>(nsuCSParam_t &param_name) {
        if (!icdParams->isMember(param_name)) return "";
        return (*icdParams)[param_name][1].asString();
    }
}

#endif //NSUKIT_JSON_ICD_WRAPPER_H
