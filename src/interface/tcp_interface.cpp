//
// Created by 56585 on 2023/8/16.
//

#include "interface/tcp_interface.h"


using namespace nsukit;


TCPCmdUItf::TCPCmdUItf() {
    mixin_ = new Mixin_VirtualRegCmd(this);
}


nsukitStatus_t TCPCmdUItf::accept(nsuInitParam_t *param) {
    tcpTimeout = param->cmd_tcp_timeout;
    tcpPort = param->cmd_tcp_port;
    ipAddr = param->cmd_ip;

    opLock.lock();
    try {
        sockGen.Init(true, tcpPort, ipAddr);
        sockGen.ConnectServer();
    } catch (...) {
        opLock.unlock();
        return nsukitStatus_t::NSUKIT_STATUS_ACCEPT_FAIL;
    }
    opLock.unlock();
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t TCPCmdUItf::close() {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t TCPCmdUItf::set_timeout(float s) {
    sockGen.recvTimeout = (int) (s * 1000);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t TCPCmdUItf::write(nsuRegAddr_t addr, nsuRegValue_t value) {
    return mixin_->_common_write(addr, value);
}


nsukitStatus_t TCPCmdUItf::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
    return mixin_->_common_read(addr, buf);
}


nsukitStatus_t TCPCmdUItf::send_bytes(nsuBytes_t &bytes) {
    return send_bytes(bytes.data(), bytes.size());
}


nsukitStatus_t TCPCmdUItf::send_bytes(nsuCharBuf_p bytes, nsuSize_t length) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    opLock.lock();
    auto tcp_status = TcpSendBytes(&sockGen, bytes, length);
    if (tcp_status == 0) res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;

//    sockGen.CloseSock(sockGen.tcpClient);
    opLock.unlock();
    return res;
}


nsukitStatus_t TCPCmdUItf::recv_bytes(nsuSize_t size, nsuCharBuf_p buf) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    opLock.lock();
    auto tcp_status = TcpRecvBytes(&sockGen, buf, size);
    if (tcp_status == 0) res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
//    sockGen.CloseSock(sockGen.tcpClient);
    opLock.unlock();
    return res;
}


TCPStreamUItf::~TCPStreamUItf() {
    if (upload_thread.joinable()) {
        stop_event.setEvent();
    }
    if (upload_thread.joinable()) {
        stop_event.setEvent();
    }
    std::map<unsigned int, Memory *>::iterator it;
    for (it = memory_dict.begin(); it != memory_dict.end(); ++it) {
        if (it->second->self_alloc) free(it->second->memory);
        delete it->second;
    }
}


nsukitStatus_t TCPStreamUItf::accept(nsuInitParam_t *param) {
    ipAddr = param->stream_ip;
    tcpPort = param->stream_tcp_port;
    block_size = param->stream_tcp_block;
    opLock.lock();
    try {
        sockGen.Init(true, tcpPort, ipAddr);
    } catch (...) {
        opLock.unlock();
        return nsukitStatus_t::NSUKIT_STATUS_ACCEPT_FAIL;
    }
    opLock.unlock();
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t TCPStreamUItf::close() {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    opLock.lock();
    try {
        sockGen.CloseSock(sockGen.tcpServer);
    } catch (...) {
        res |= nsukitStatus_t::NSUKIT_STATUS_ITF_FAIL;
    }
    opLock.unlock();
    return res;
}


nsukitStatus_t TCPStreamUItf::set_timeout(float s) {
    tcpTimeout = s;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsuMemory_p TCPStreamUItf::alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf) {
    auto mem = new Memory{};
    if (buf == nullptr) {
        mem->memory = (nsuMemory_p) malloc(length);
        mem->self_alloc = true;
    } else {
        mem->memory = (nsuMemory_p) buf;
        mem->self_alloc = false;
    }

    memset(mem->memory, length, 1);
    mem->idx = memory_index;
    mem->mem_size = length;
    mem->finish_event.setEvent();

    memory_dict[memory_index] = mem;
    memory_index++;
    return mem;
}


nsukitStatus_t TCPStreamUItf::free_buffer(nsuMemory_p fd) {
    auto mem = (Memory *) fd;
    if (!mem->finish_event.isSet()) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
    }
    mem->finish_event.resetEvent();
    if (mem->self_alloc) free(mem->memory);
    memory_dict.erase(mem->idx);
    delete mem;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsuVoidBuf_p TCPStreamUItf::get_buffer(nsuMemory_p fd, nsuStreamLen_t length) {
    auto mem = (Memory *) fd;
    return (nsuVoidBuf_p) (mem->memory);
}


nsukitStatus_t
TCPStreamUItf::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    auto mem = (Memory *) fd;
    { // set memory param
        std::unique_lock<std::mutex> lock(mem->mtx);
        if (mem->mem_size < length + offset) {
            return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
        }
        mem->need_size = length;
        mem->using_size = 0;
        mem->error_msg = "";
    }

    {
        std::unique_lock<std::mutex> lock(opLock);
        if ((!sockGen.tcpClientConnected) && sockGen.AcceptClient()==-1) return nsukitStatus_t::NSUKIT_STATUS_ACCEPT_FAIL;
    }

    auto thread = std::thread(_recv, &sockGen, &stop_event, mem, block_size, length, offset);
    if (upload_thread.joinable()) upload_thread.join();
    upload_thread = std::move(thread);
    return res;
}


void TCPStreamUItf::_recv(SocketGenerator *sock, ThreadSafeEvent *stop_event, Memory *mem, uint32_t block,
                          nsuStreamLen_t length, nsuStreamLen_t offset) {
    auto buffer = (nsuCharBuf_p) (mem->memory) + offset;
    mem->finish_event.resetEvent();
    stop_event->resetEvent();
    int stLen = 0;
    int totalLen = 0;
    int cur_count;

    while (length) {
        if (stop_event->isSet()) {
            mem->finish_event.setEvent();
            break;
        }
        cur_count = length - totalLen > block ? block : length - totalLen;
        stLen = sock->RecvData(sock->tcpClient, buffer + totalLen, cur_count);
        if (stLen == 0) {
            // server connect interrupt
            std::unique_lock<std::mutex> lock(mem->mtx);
            mem->error_msg += "connect interrupt error";
            break;
        } else if (stLen > 0) {
            std::unique_lock<std::mutex> lock(mem->mtx);
            totalLen += stLen;
            mem->using_size = totalLen;
            if (totalLen >= length) {
                break;
            }
        }
    }
    mem->finish_event.setEvent();
    stop_event->setEvent();
}


nsukitStatus_t TCPStreamUItf::wait_stream(nsuMemory_p fd, float timeout) {
    auto mem = (Memory *) fd;
    if (memory_dict.count(mem->idx) == 0) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    mem->finish_event.waitForEvent(static_cast<int>(timeout));

    std::unique_lock<std::mutex> lock(mem->mtx);
    if (mem->using_size < mem->need_size) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
    } else if (!mem->error_msg.empty()) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL;
    } else {
        return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    }

}


nsukitStatus_t TCPStreamUItf::break_stream(nsuMemory_p fd) {
    stop_event.setEvent();
    if (upload_thread.joinable()) upload_thread.join();
    stop_event.resetEvent();
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t
TCPStreamUItf::stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    auto res = open_recv(chnl, fd, length, offset);
    if (stop_event == nullptr) stop_event = []() {return false;};
    res = nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
    while (!stop_event() && res!=nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        res = wait_stream(fd, 1.);
    }
    return res;
}


nsukitStatus_t
TCPStreamUItf::open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    return I_BaseStreamUItf::open_send(chnl, fd, length, offset);
}


nsukitStatus_t
TCPStreamUItf::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    return I_BaseStreamUItf::stream_send(chnl, fd, length, offset, stop_event, timeout, flag);
}
