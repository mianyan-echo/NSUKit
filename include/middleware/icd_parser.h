//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_ICD_PARSER_H
#define NSUKIT_ICD_PARSER_H


#include <fstream>
#include <cassert>
#include "base_mw.h"


namespace nsukit {
#define ICD_Uint8                     "uint8"
#define ICD_Int8                      "int8"
#define ICD_Uint16                    "uint16"
#define ICD_Int16                     "int16"
#define ICD_Uint32                    "uint32"
#define ICD_Int32                     "int32"
#define ICD_Float                     "float"
#define ICD_Double                    "double"
#define ICD_File                      "file"
#define ICD_FileLength                "filelength"
#define ICD_ParamFlag_File            "__file__"
#define ICD_ParamFlag_FileLength      "__filelength__"


    static std::map<std::string, int> TypeSizeMap = {
            {ICD_Uint8,      1},
            {ICD_Int8,       1},
            {ICD_Uint16,     2},
            {ICD_Int16,      2},
            {ICD_Uint32,     4},
            {ICD_Int32,      4},
            {ICD_Float,      4},
            {ICD_FileLength, 4},
            {ICD_Double,     8}
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

    NSU_DLLEXPORT bool StringEndWith(const std::string& str, const std::string& tail);

    NSU_DLLEXPORT bool StringStartWith(const std::string& str, const std::string& head);


    class JsonWrapper;

    class NSU_DLLEXPORT ICDRegMw : public I_BaseRegMw {
    protected:
        I_BaseCmdUItf *cs_itf;
        std::string icdPath{};
        JsonWrapper *icd;
        bool checkFeedback = false;

        nsukitStatus_t EnableParam(nsuCSName_t cname, nsuCharBuf_p buf);

        nsukitStatus_t set_param_by_tname(nsuCSParam_t pname, std::string type, nsuCharBuf_p buf);

    public:

        explicit ICDRegMw(I_BaseCmdUItf *itf);

        ~ICDRegMw();

        nsukitStatus_t config(nsuInitParam_t *param) override;

        CommandPack *FmtCommand(const std::string &cmd_name, const std::string &file_path = "");

        uint8_t get_param(nsuCSParam_t &param_name, uint8_t _default = 0) override;

        int8_t get_param(nsuCSParam_t &param_name, int8_t _default = 0) override;

        uint16_t get_param(nsuCSParam_t &param_name, uint16_t _default = 0) override;

        int16_t get_param(nsuCSParam_t &param_name, int16_t _default = 0) override;

        uint32_t get_param(nsuCSParam_t &param_name, uint32_t _default = 0) override;

        int32_t get_param(nsuCSParam_t &param_name, int32_t _default = 0) override;

        double get_param(nsuCSParam_t &param_name, double _default = 0.) override;

        float get_param(nsuCSParam_t &param_name, float _default = 0.) override;

        std::string get_param(nsuCSParam_t &param_name, std::string _default = "") override;

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
    };
}


#endif //NSUKIT_ICD_PARSER_H
