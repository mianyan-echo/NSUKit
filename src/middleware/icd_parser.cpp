//
// Created by 56585 on 2023/8/14.
//

#include "json_icd_wrapper.h"

using namespace nsukit;

/**
 * 检查指令反馈
 * @param data
 */
inline static void *CheckFeedback(char *data) {
    StandardCmdFeedback feedback;
    memcpy(&feedback, data, sizeof(feedback));
    if (feedback.packHead != 0xCFCFCFCF) {
        std::cout << "Receive instruction header is misplaced" << std::endl;
        return nullptr;
    }
    if (feedback.execSucceed != 0) {
        std::cout << "The command was sent successfully but failed to execute" << std::endl;
        return nullptr;
    }
    std::cout << "Command sent successfully" << std::endl;
    return nullptr;
}


bool JsonWrapper::InitICD(std::string_view path) {
    // Get the input stream of icd.json
    std::ifstream _in(path.data(), std::ios::binary);
    if (!_in.is_open()) {
        std::cout << "can not find icd file: " << path << std::endl;
        throw std::runtime_error("can not find icd file");
    }

    // Parse json file
    if (ICDReader.parse(_in, icdRoot)) {
        icdCommands = &icdRoot["command"];
        if (icdRoot.isMember("param")) {
            icdParams = &icdRoot["param"];
        }
    } else {
        std::cout << "file: " << path << "Wrong format!" << std::endl;
        throw std::runtime_error("icd file format error");
    }
    _in.close();

    return true;
}


ICDRegMw::ICDRegMw(I_BaseCmdUItf *itf) {
    cs_itf = itf;
    icd = new JsonWrapper();
}


ICDRegMw::~ICDRegMw() {
    delete icd;
}


nsukitStatus_t ICDRegMw::config(nsuInitParam_t *param) {
    icdPath = param->icd_path;
    checkFeedback = param->check_cs_recv;

    try {
        icd->InitICD(icdPath);
    } catch (...) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }

    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


uint8_t ICDRegMw::get_param(nsuCSParam_t &param_name, uint8_t _default) {
    try {
        return icd->get_icd_param<uint8_t>(param_name);
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return _default;
    }
}

int8_t ICDRegMw::get_param(nsuCSParam_t &param_name, int8_t _default) {
    try {
        return icd->get_icd_param<int8_t>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}

uint16_t ICDRegMw::get_param(nsuCSParam_t &param_name, uint16_t _default) {
    try {
        return icd->get_icd_param<uint16_t>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}

int16_t ICDRegMw::get_param(nsuCSParam_t &param_name, int16_t _default) {
    try {
        return icd->get_icd_param<int16_t>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}

uint32_t ICDRegMw::get_param(nsuCSParam_t &param_name, uint32_t _default) {
    try {
        return icd->get_icd_param<uint32_t>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}

int32_t ICDRegMw::get_param(nsuCSParam_t &param_name, int32_t _default) {
    try {
        return icd->get_icd_param<int32_t>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}

double ICDRegMw::get_param(nsuCSParam_t &param_name, double _default) {
    try {
        return icd->get_icd_param<double>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}

float ICDRegMw::get_param(nsuCSParam_t &param_name, float _default) {
    try {
        return icd->get_icd_param<float>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}

std::string ICDRegMw::get_param(nsuCSParam_t &param_name, std::string _default) {
    try {
        return icd->get_icd_param<std::string>(param_name);
    } catch (std::runtime_error &e) {
        return _default;
    }
}


nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const uint8_t &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const int8_t &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const uint16_t &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const int16_t &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const uint32_t &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const int32_t &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const double &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const float &value) {
    return icd->set_icd_param(param_name, value);
}

nsukitStatus_t ICDRegMw::set_param(nsuCSParam_t &param_name, const std::string &value) {
    return icd->set_icd_param(param_name, value);
}


nsukitStatus_t ICDRegMw::execute(nsuCSName_t cname) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    CommandPack *pCommandPack;
    try {
        pCommandPack = FmtCommand(cname);
    } catch (...) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }

    res |= cs_itf->send_bytes(pCommandPack->bytes, pCommandPack->length);

    if (pCommandPack->recvLen > 0) {
        char *result = new char[pCommandPack->recvLen]();
        res |= cs_itf->recv_bytes(pCommandPack->recvLen, result);
        if (checkFeedback) CheckFeedback(result);
        res |= EnableParam(cname, result);
        delete[] result;
    }
    DeleteCommandPack(pCommandPack);
    return res;
}


/**
 * 格式化一个寄存器的值
 * @param reg
 * @param value
 * @return
 */
CommandPack *JsonWrapper::FmtRegister(const Json::Value &reg, const Json::Value &value) {
    if (value.isString() && StringStartWith(value.asString(), "0x")) {
        // 对于 "0x5F5F5F5F"
        auto _value = (uint32_t) std::stoll(value.asString(), nullptr, 16);
        return FmtRegister(reg, _value);
    } else if (value.isString() && StringStartWith(value.asString(), "0b")) {
        // 对于 "0b10101011"
        auto _value = std::stoll(value.asString().substr(2), nullptr, 2);
        return FmtRegister(reg, _value);
    } else if (value.isString() && reg[0].asString() == ICD_File) {
        // 对于 file文件类型
        return FmtFile(value.asString());
    } else if (value.isString() && reg[0].asString() == ICD_FileLength) {
        // 对于文件大小类型
        return FmtFile(value.asString(), true);
    } else if (value.isNumeric()) {
        // 对于全体数字
        std::string value_type = reg[0].asString();
        if (value_type == ICD_Float) {
            auto _value = value.asFloat();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_Double) {
            auto _value = value.asDouble();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_Uint8) {
            auto _value = (uint8_t) value.asUInt();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_Int8) {
            auto _value = (int8_t) value.asInt();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_Uint16) {
            auto _value = (uint16_t) value.asUInt();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_Int16) {
            auto _value = (int16_t) value.asInt();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_Uint32) {
            auto _value = (uint32_t) value.asUInt();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_Int32) {
            auto _value = (int32_t) value.asInt();
            return FmtRegister(reg, _value);
        } else if (value_type == ICD_FileLength) {
            auto _value = (uint32_t) value.asInt();
            return FmtRegister(reg, _value);
        } else {
            std::cout << "Unrecognized value type " << value.toStyledString() << std::endl;
            throw "Unrecognized register value type";
        }
    } else {
        std::cout << "Unrecognized value type " << value.toStyledString() << std::endl;
        throw "Unrecognized register value type";
    }
}

/**
 * 针对string类型输入格式化为CommandPack
 * @param reg
 * @param value
 * @return
 */
CommandPack *JsonWrapper::FmtRegister(const Json::Value &reg, const std::string &value) {
    if (StringStartWith(value, "0x")) {
        unsigned int _value = std::stoll(value, nullptr, 16);
        return FmtRegister(reg, _value);
    } else if (StringStartWith(value, "0b")) {
        unsigned short _value = std::stoll(value, nullptr, 2);
        return FmtRegister(reg, _value);
    } else {
        std::cout << "Unrecognized value type " << value << std::endl;
        throw "Unrecognized register value type";
    }
}


/**
 * 读取文件，以CommandPack的形式返回
 * @param file_path 文件路径
 * @param need_size
 * @return 独处的CommandPack
 */
CommandPack *JsonWrapper::FmtFile(const std::string &file_path, bool need_size) {
    CommandPack *fileData = nullptr;
    std::ifstream _in(file_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (_in.is_open()) {
        fileData = new CommandPack;
        if (need_size) {
            fileData->length = TypeSizeMap[ICD_Uint32];
            fileData->bytes = new char[fileData->length + 1]();
            uint32_t fileLength = (uint32_t) _in.tellg();
            memcpy(fileData->bytes, &fileLength, fileData->length);
        } else {
            fileData->length = _in.tellg();
            fileData->bytes = new char[fileData->length + 1]();
            _in.seekg(0, std::ios::beg);
            _in.read(fileData->bytes, fileData->length);
        }
    } else {
        std::cout << "文件路径 " << file_path << " 错误" << std::endl;
    }
    return fileData;
}


/**
 * 指令组包，组为一个CommandPack
 * @param cmd_name
 * @param file_path
 * @return
 */
CommandPack *ICDRegMw::FmtCommand(const std::string &cmd_name, const std::string &file_path) {
    Json::Value command;
    std::vector<CommandPack *> registerList;
    CommandPack *regTemp;
    nsuSize_t recv_len = 0;
    auto resCommandPack = new CommandPack;
    uint32_t commandLength = 0;

    if (!icd->icdCommands->isMember(cmd_name)) {
        std::cout << "Not find command: " << cmd_name << std::endl;
        throw std::runtime_error("指令名称在icd中不存在");
    }

    // 生成要发送的指令
    command = (*(icd->icdCommands))[cmd_name]["send"];
    for (auto &reg: command) {
        if (reg.isString() && reg.asString() == ICD_ParamFlag_File) {
            regTemp = icd->FmtFile(file_path);
        } else if (reg.isString() && reg.asString() == ICD_ParamFlag_FileLength) {
            regTemp = icd->FmtFile(file_path, true);
        } else if (reg.isString() && icd->icdParams->isMember(reg.asString())) {
            reg = (*icd->icdParams)[reg.asString()];
            regTemp = icd->FmtRegister(reg, reg[1]);
        } else if (reg.isArray()) {
            regTemp = icd->FmtRegister(reg, reg[1]);
        } else {
            throw std::runtime_error("不受支持的寄存器写法");
        }
        assert(regTemp->bytes != nullptr);
        registerList.emplace_back(regTemp);
        commandLength += regTemp->length;
    }

    // 计算要接收的指令长度
    command = (*(icd->icdCommands))[cmd_name]["recv"];
    for (auto &reg: command) {
        if (reg.isString() && reg.asString() == ICD_ParamFlag_File) {
            throw std::runtime_error("不受支持的寄存器写法");
        } else if (reg.isString() && reg.asString() == ICD_ParamFlag_FileLength) {
            throw std::runtime_error("不受支持的寄存器写法");
        } else if (reg.isString() && icd->icdParams->isMember(reg.asString())) {
            reg = (*(icd->icdParams))[reg.asString()];
            recv_len += TypeSizeMap[reg[0].asString()];
        } else if (reg.isArray()) {
            recv_len += TypeSizeMap[reg[0].asString()];
        } else {
            throw std::runtime_error("不受支持的寄存器写法");
        }
    }

    // 将获取到的二进制寄存器列表拼接为一个CommandPack
    resCommandPack->bytes = new char[commandLength + 1];
    resCommandPack->length = commandLength;
    resCommandPack->recvLen = recv_len;
    char *flag = resCommandPack->bytes;
    for (auto &reg: registerList) {
        memcpy(flag, reg->bytes, reg->length);
        flag += reg->length;
        DeleteCommandPack(reg);
    }
    // 修改指令长度
    memcpy(resCommandPack->bytes + 12, &commandLength, 4);

    return resCommandPack;
}

nsukitStatus_t ICDRegMw::EnableParam(nsuCSName_t cname, nsuCharBuf_p buf) {
    Json::Value command;
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;

    if (!icd->icdCommands->isMember(cname)) {
        std::cout << "Not find command: " << cname << std::endl;
        throw std::runtime_error("指令名称在icd中不存在");
    }

    // 遍历recv中的所有参数
    command = (*(icd->icdCommands))[cname]["recv"];
    auto ptr = (nsuCharBuf_p )buf;
    for (auto &reg: command) {
        if (reg.isString() && reg.asString() == ICD_ParamFlag_File) {
            res |= nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        } else if (reg.isString() && reg.asString() == ICD_ParamFlag_FileLength) {
            res |= nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        } else if (reg.isString() && icd->icdParams->isMember(reg.asString())) {
            auto _pack = (*(icd->icdParams))[reg.asString()];
            auto _width = TypeSizeMap[_pack[0].asString()];
            res |= set_param_by_tname(reg.asString(), _pack[0].asString(), ptr);
            ptr += _width;
        } else if (reg.isArray()) {
            ptr += TypeSizeMap[reg[0].asString()];
        } else {
            res |= nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
    }
    return res;
}

nsukitStatus_t ICDRegMw::set_param_by_tname(nsuCSParam_t pname, std::string type, nsuCharBuf_p buf) {
    if (type == ICD_Float) {
        auto _value = *(float *)buf;
        return icd->set_icd_param(pname, _value);
    } else if (type == ICD_Double) {
        auto _value = *(double *)buf;
        return icd->set_icd_param(pname, _value);
    } else if (type == ICD_Uint8) {
        auto _value = *(uint8_t *)buf;
        return icd->set_icd_param(pname, _value);
    } else if (type == ICD_Int8) {
        auto _value = *(int8_t *)buf;
        return icd->set_icd_param(pname, _value);
    } else if (type == ICD_Uint16) {
        auto _value = *(uint16_t *)buf;
        return icd->set_icd_param(pname, _value);
    } else if (type == ICD_Int16) {
        auto _value = *(int16_t *)buf;
        return icd->set_icd_param(pname, _value);
    } else if (type == ICD_Uint32) {
        auto _value = *(uint32_t *)buf;
        return icd->set_icd_param(pname, _value);
    } else if (type == ICD_Int32) {
        auto _value = *(int32_t *) buf;
        return icd->set_icd_param(pname, _value);
    }
    return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
}


uint32_t nsukit::GetFileSize(const std::string &file_path) {
    uint32_t size = 0;
    std::ifstream _in(file_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (_in.is_open()) {
        size = _in.tellg();
    }
    return size;
}

/**
 * 判断一个字符串str是不是以tail结尾的
 * @param str 目标字符串
 * @param tail 目标结尾
 * @return bool 是或否
 */
bool nsukit::StringEndWith(const std::string& str, const std::string& tail) {
    return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

/**
 * 判断一个字符串str是不是以head开头的
 * @param str 目标字符串
 * @param head 目标开头
 * @return bool 是或否
 */
bool nsukit::StringStartWith(const std::string& str, const std::string& head) {
    return str.compare(0, head.size(), head) == 0;
}

/**
 * 删除通过new申请的CommandPack
 * @param pack 在堆上的CommandPack的指针
 */
void nsukit::DeleteCommandPack(CommandPack *pack) {
    delete[] pack->bytes;
    delete pack;
}
