//
// Created by 56585 on 2023/12/27.
//

#include "high_level/HardwareSync.h"
#include <string>
#include <sstream>


/**
 *
 * @param cmd_str
 * @param cmds
 * @return
 */
nsukitStatus_t parse_cmd_str(char *cmd_str, std::vector<std::string>& cmds) {
    if (cmd_str == nullptr) {
        char hsync_fmt[] = HSYNC_DEFAULT_CMD_FMT;
        cmd_str = hsync_fmt;
    }
    auto string = std::string(cmd_str);

    std::stringstream ss(string);
    std::string token;

    while (std::getline(ss, token, HSYNC_CMD_FMT_DELIM)) {
        cmds.emplace_back(token);
    }
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param slave_list
 * @param cmd
 * @return
 */
nsukitStatus_t parallel_execute(std::vector<nsukit::BaseKit *>& slave_list, const std::string& cmd) {
    nsukitStatus_t res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    for(const auto &elem : slave_list) {
        res |= elem->execute(cmd);
    }
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t nsukit::hl_hsync_sync(nsukit::BaseKit *master, nsukit::BaseKit **slaves, int slave_num, char *cmd_str) {
    nsukitStatus_t res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    std::vector<std::string> cmds;
    std::vector<nsukit::BaseKit *> slave_list;

    parse_cmd_str(cmd_str, cmds);
    if (cmds.size() != 6) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    slave_list.reserve(slave_num);
    for (int i = 0; i < slave_num; i++) {
        slave_list.push_back(slaves[i]);
    }

    res |= parallel_execute(slave_list, cmds[0]);
    res |= master->execute(cmds[1]);
    res |= parallel_execute(slave_list, cmds[2]);
    res |= master->execute(cmds[3]);
    res |= parallel_execute(slave_list, cmds[4]);
    res |= master->execute(cmds[5]);
    return res;
}
