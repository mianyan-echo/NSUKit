//
// Created by 56585 on 2023/8/14.
//

#include "middleware/icd_parser.h"

using namespace nsukit;

/**
 * 检查指令反馈
 * @param data
 */
inline static void *CheckFeedback(char *data) {
    StandardCmdFeedback feedback;
    memcpy(&feedback, data, sizeof(feedback));
    if (feedback.packHead != 0xCFCFCFCF) {
        std::cout << "接收指令包头错位" << std::endl;
        return nullptr;
    }
    if (feedback.execSucceed != 0) {
        std::cout << "指令发送成功但执行失败" << std::endl;
        return nullptr;
    }
    std::cout << "指令发送成功" << std::endl;
    return nullptr;
}

void ICDRegMw::ShowInfo() {
    if (serverIP.length()) {
        std::cout << "Socket: " << serverIP << ":" << serverPort << "\n";
    } else {
        std::cout << "Axi: " << dmaBoard << "\n";
    }
}

bool ICDRegMw::InitICD(std::string_view path) {
    //获取icd.json的输入流
    std::ifstream _in(icdPath, std::ios::binary);
    if (!_in.is_open()) {
        std::cout << "can not find icd file: " << icdPath << std::endl;
        throw "can not find icd file";
    }

    // 解析json文件
    if (ICDReader.parse(_in, icdRoot)) {
        icdCommands = &icdRoot["command"];
        if (icdRoot.isMember("param")) {
            icdParams = &icdRoot["param"];
        }
    } else {
        std::cout << "文件: " << icdPath << "格式有误!" << std::endl;
        throw "icd文件格式错误";
    }
    _in.close();

    return true;
}


ICDRegMw::ICDRegMw(BaseKit *kit, std::string_view file_name) {

}


//ICDRegMw::ICDRegMw(std::string_view path, int dma_board): icdPath(path),
//                                                          sendFlag("toaxi"),
//                                                          dmaBoard(dma_board) {
//    InitICD(path);
//}
//
//
///**
// * 解析icd文件，指令组包
// * @param path icd文件路径
// */
//ICDRegMw::ICDRegMw(std::string_view path, std::string_view server_ip, unsigned short int port) : icdPath(path),
//                                                                                                 serverIP(server_ip),
//                                                                                                 sendFlag("socket"),
//                                                                                                 serverPort(port) {
//    InitICD(path);
//}

ICDRegMw::~ICDRegMw() = default;


nsukitStatus_t ICDRegMw::config(nsuInitParam_t *param) {
    auto _param = (nsuInitParam_t *)param;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 * 修改指令目标ip与port
 * @param _ip
 * @param port
 * @return
 */
bool ICDRegMw::SetServerAddr(const std::string &_ip, unsigned short port) {
    serverIP = _ip;
    if (port != 0) serverPort = port;
    return true;
}


/**
 * 格式化一个寄存器的值
 * @param reg
 * @param value
 * @return
 */
CommandPack *ICDRegMw::FmtRegister(const Json::Value &reg, const Json::Value &value) {
    if (value.isString() && StringStartWith(value.asString(), "0x")) {
        // 对于 "0x5F5F5F5F"
        auto _value = (uint32_t) std::stoll(value.asString(), nullptr, 16);
        return FmtRegister(reg, _value);
    } else if (value.isString() && StringStartWith(value.asString(), "0b")) {
        // 对于 "0b10101011"
        auto _value = std::stoll(value.asString().substr(2), nullptr, 2);
        return FmtRegister(reg, _value);
    } else if (value.isString() && reg[1].asString() == ICD_File) {
        // 对于 file文件类型
        return FmtFile(value.asString());
    } else if (value.isNumeric()) {
        // 对于全体数字
        std::string value_type = reg[1].asString();
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
        } else {
            std::cout << "未识别的值类型 " << value.toStyledString() << std::endl;
            throw "未识别的寄存器值类型";
        }
    } else {
        std::cout << "未识别的值类型 " << value.toStyledString() << std::endl;
        throw "未识别的寄存器值类型";
    }
}

/**
 * 针对string类型输入格式化为CommandPack
 * @param reg
 * @param value
 * @return
 */
CommandPack *ICDRegMw::FmtRegister(const Json::Value &reg, const std::string &value) {
    if (StringStartWith(value, "0x")) {
        unsigned int _value = std::stoll(value, nullptr, 16);
        return FmtRegister(reg, _value);
    } else if (StringStartWith(value, "0b")) {
        unsigned short _value = std::stoll(value, nullptr, 2);
        return FmtRegister(reg, _value);
    } else {
        std::cout << "未识别的值类型 " << value << std::endl;
        throw "未识别的寄存器值类型";
    }
}


/**
 * 读取文件，以CommandPack的形式返回
 * @param file_path 文件路径
 * @param need_size
 * @return 独处的CommandPack
 */
CommandPack *ICDRegMw::FmtFile(const std::string &file_path, bool need_size) {
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
    auto resCommandPack = new CommandPack;
    uint32_t commandLength = 0;

    if (icdCommands->isMember(cmd_name)) {
        command = (*icdCommands)[cmd_name];
        for (auto &reg: command) {
            if (reg.isString() && reg.asString() == ICD_ParamFlag_File) {
                regTemp = FmtFile(file_path);
            } else if (reg.isString() && reg.asString() == ICD_ParamFlag_FileLength) {
                regTemp = FmtFile(file_path, true);
            } else if (reg.isString() && icdParams->isMember(reg.asString())) {
                reg = (*icdParams)[reg.asString()];
                regTemp = FmtRegister(reg, reg[2]);
            } else if (reg.isArray()) {
                regTemp = FmtRegister(reg, reg[2]);
            } else {
                throw "不受支持的寄存器写法";
            }
            assert(regTemp->bytes != nullptr);
            registerList.emplace_back(regTemp);
            commandLength += regTemp->length;
        }
    } else {
        std::cout << "Not find command: " << cmd_name << std::endl;
        throw "指令名称在icd中不存在";
    }

    // 将获取到的二进制寄存器列表拼接为一个CommandPack
    resCommandPack->bytes = new char[commandLength + 1];
    resCommandPack->length = commandLength;
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
bool nsukit::StringEndWith(std::string_view str, std::string_view tail) {
    return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

/**
 * 判断一个字符串str是不是以head开头的
 * @param str 目标字符串
 * @param head 目标开头
 * @return bool 是或否
 */
bool nsukit::StringStartWith(std::string_view str, std::string_view head) {
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
