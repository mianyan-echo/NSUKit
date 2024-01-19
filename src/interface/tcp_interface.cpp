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
    sockGen.recvTimeout = (int)(s*1000);
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


nsukitStatus_t TCPStreamUItf::accept(nsuInitParam_t *param) {
    return U_Interface::accept(param);
}


nsukitStatus_t TCPStreamUItf::close() {
    return U_Interface::close();
}


nsukitStatus_t TCPStreamUItf::set_timeout(float s) {
    return U_Interface::set_timeout(s);
}


nsuMemory_p TCPStreamUItf::alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf) {
    return I_BaseStreamUItf::alloc_buffer(length, buf);
}


nsukitStatus_t TCPStreamUItf::free_buffer(nsuMemory_p fd) {
    return I_BaseStreamUItf::free_buffer(fd);
}


nsuVoidBuf_p TCPStreamUItf::get_buffer(nsuMemory_p fd, nsuStreamLen_t length) {
    return I_BaseStreamUItf::get_buffer(fd, length);
}


nsukitStatus_t
TCPStreamUItf::open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    return I_BaseStreamUItf::open_send(chnl, fd, length, offset);
}


nsukitStatus_t
TCPStreamUItf::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    return I_BaseStreamUItf::open_recv(chnl, fd, length, offset);
}


nsukitStatus_t TCPStreamUItf::wait_stream(nsuMemory_p fd, float timeout) {
    return I_BaseStreamUItf::wait_stream(fd, timeout);
}


nsukitStatus_t TCPStreamUItf::break_stream(nsuMemory_p fd) {
    return I_BaseStreamUItf::break_stream(fd);
}


nsukitStatus_t
TCPStreamUItf::stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    return I_BaseStreamUItf::stream_recv(chnl, fd, length, offset, stop_event, timeout, flag);
}


nsukitStatus_t
TCPStreamUItf::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    return I_BaseStreamUItf::stream_send(chnl, fd, length, offset, stop_event, timeout, flag);
}
