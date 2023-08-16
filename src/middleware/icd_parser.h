//
// Created by 56585 on 2023/8/14.
//

#ifndef NSUKIT_ICD_PARSER_H
#define NSUKIT_ICD_PARSER_H


#include <fstream>
#include <cassert>
#include <vector>
#include <map>
#include "base_mw.h"
#include "json/json.h"
//#include "c_socket.h"
//#include "TCPoverAXI.h"


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
    uint32_t length = 0;
};

/**
 * \name StatusPack
 * \note
 * 接收并解析返回的状态信息
 */
struct StatusPack {
    uint32_t packHead = 0;
    uint32_t packId = 0;
    uint32_t packNum = 0;
    uint32_t packLength = 0;
    uint32_t fpgaVersion = 0;
    uint32_t fpgaVoltage = 0;
    uint32_t fpgaTemp = 0;
    uint32_t recvPPSNum = 0;              // 接收到的PPS信号数量
    uint32_t adc1_OverVoltage = 0;        // ADC1的过压报警信号 一次报警后置位，AGX读取后复位
    uint32_t adc2_OverVoltage = 0;        // ADC2的过压报警信号 一次报警后置位，AGX读取后复位
    uint32_t adc3_OverVoltage = 0;        // ADC3的过压报警信号 一次报警后置位，AGX读取后复位
    uint32_t adc4_OverVoltage = 0;        // ADC4的过压报警信号 一次报警后置位，AGX读取后复位
    uint32_t xadc1_Amplitude = 0;         // 慢速采集信号1幅值
    uint32_t xadc2_Amplitude = 0;         // 慢速采集信号2幅值
    uint32_t powerState = 0;              // 电源状态
    uint32_t adcConfigured = 0;           // ADC配置成功标志
    uint32_t dacConfigured = 0;           // DAC配置成功标志
    uint32_t ramLoaded = 0;               // 缓存波形加载成功标志
    uint32_t gpsTime = 0;                 // GPS的时分秒  16进制表示即可
    uint32_t adc1_Maximum = 0;            // ADC1采集信号的时域最大值  当前PRT保持，下个PRT覆盖
    uint32_t adc2_Maximum = 0;            // ADC2采集信号的时域最大值  当前PRT保持，下个PRT覆盖
    uint32_t adc3_Maximum = 0;            // ADC3采集信号的时域最大值  当前PRT保持，下个PRT覆盖
    uint32_t adc4_Maximum = 0;            // ADC4采集信号的时域最大值  当前PRT保持，下个PRT覆盖
};

void DeleteCommandPack(CommandPack *pack);


class ICDRegMw: public I_BaseRegMw{
private:
    std::string serverIP{};
    int dmaBoard{};
    unsigned short int serverPort{};
    std::string sendFlag{};

protected:
    Json::Reader ICDReader;  //icd.json读取器
    Json::Value icdRoot{};
    Json::Value *icdParams{};
    Json::Value *icdCommands{};
    std::string icdPath{};

    template<class T>
    CommandPack *FmtRegister(const Json::Value &reg, const T &value);

    CommandPack *FmtRegister(const Json::Value &reg, const Json::Value &value);

    CommandPack *FmtRegister(const Json::Value &reg, const std::string &value);

    CommandPack *FmtFile(const std::string &file_path, bool need_size = false);

public:
//    explicit ICDRegMw(std::string_view path, std::string_view server_ip, unsigned short int port);
//
//    explicit ICDRegMw(std::string_view path, int dma_board);

    explicit ICDRegMw(BaseKit *kit, std::string_view file_name="icd.json");

    ~ICDRegMw();

    nsukitStatus_t config(nsuAcceptParam_t* param) override;

    void ShowInfo();

    bool InitICD(std::string_view path);

    bool SetServerAddr(const std::string &_ip, unsigned short int port=0);

//    bool SendCommand(const std::string &cmd_name, const std::string &file_path = "", const bool &need_feedback = true,
//                     const int &feedback_length = 20, const int &wait = 0);

    CommandPack *FmtCommand(const std::string &cmd_name, const std::string &file_path = "");

//    StatusPack GetStatus(StatusPack *statusBuf = nullptr);

    template<class T>
    bool SetParamValue(const std::string &param_name, const T &value);

    template<class T>
    T GetParamValue(const std::string &param_name);
};

uint32_t GetFileSize(const std::string &file_path);

bool StringEndWith(std::string_view str, std::string_view tail);

bool StringStartWith(std::string_view str, std::string_view head);


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
    size = TypeSizeMap[reg[1].asString()];
    res->bytes = new char[size + 1]();
    res->length = size;
    memcpy(res->bytes, &value, size);
    return res;
}


/**
 * 修改某个参数的值
 *
 * @tparam T
 * @param param_name
 * @param value
 * @return
 */
template<class T>
bool ICDRegMw::SetParamValue(const std::string &param_name, const T &value) {
    Json::Value *param = nullptr;
    if (!icdParams->isMember(param_name)) return false;
    param = &(*icdParams)[param_name];
    (*param)[2] = Json::Value(value);
    return true;
}


/**
 * 获取参数值
 *
 * @tparam T
 * @param param_name
 * @return
 */
template<class T>
T ICDRegMw::GetParamValue(const std::string &param_name) {
    Json::Value reg, value;
    T _value;
    if (!icdParams->isMember(param_name)) throw std::runtime_error("此参数不存在");
    reg = (*icdParams)[param_name];
    value = reg[2];

    if (value.isString() && StringStartWith(value.asString(), "0x")) {
        _value = (T) std::stoll(value.asString(), nullptr, 16);
    } else if (value.isString() && StringStartWith(value.asString(), "0b")) {
        _value = (T) std::stoll(value.asString().substr(2), nullptr, 2);
    } else if (value.isString() && reg[1].asString() == ICD_File) {
        _value = (T) value.asString();
    } else if (value.isNumeric()) {
        std::string value_type = reg[1].asString();
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
inline std::string ICDRegMw::GetParamValue<std::string> (const std::string &param_name) {
    if (!icdParams->isMember(param_name)) throw std::runtime_error("此参数不存在");
    return (*icdParams)[param_name][2].asString();
}

#endif //NSUKIT_ICD_PARSER_H
