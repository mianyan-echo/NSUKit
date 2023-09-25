//
// Created by 56585 on 2023/8/17.
//

#include "interface/sim_interface.h"

using namespace nsukit;

SimStreamUItf::SimStreamUItf() = default;

SimCmdUItf::SimCmdUItf() = default;


nsukitStatus_t SimCmdUItf::accept(nsuInitParam_t *param) {
    sim_target = param->sim_target;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @return
 */
nsukitStatus_t SimCmdUItf::close() {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param s
 * @return
 */
nsukitStatus_t SimCmdUItf::set_timeout(float s) {
    simTimeout = s;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimCmdUItf::send_bytes(nsuBytes_t &bytes) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimCmdUItf::send_bytes(nsuCharBuf_p bytes, nsuSize_t length) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimCmdUItf::recv_bytes(nsuSize_t size, nsuCharBuf_p buf) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param addr
 * @param value
 * @return
 */
nsukitStatus_t SimCmdUItf::write(nsuRegAddr_t addr, nsuRegValue_t value) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


/**
 *
 * @param addr
 * @param buf
 * @return
 */
nsukitStatus_t SimCmdUItf::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
    if (buf == nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    memset(buf, 1, sizeof(nsuRegValue_t ));
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimStreamUItf::accept(nsuInitParam_t *param) {
    sim_target = param->sim_target;
    sim_stream_func = param->sim_stream_func;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t SimStreamUItf::close() {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t SimStreamUItf::set_timeout(float s) {
    return U_Interface::set_timeout(s);
}

nsuMemory_p SimStreamUItf::alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf) {
    bool self_malloc = false;
    if (buf == nullptr) {
        buf = malloc(length);
        self_malloc = true;
    }
    auto memory = new DMAMemory{length, self_malloc, buf};
    fdMap[memory] = memory;
    return memory;
}

nsukitStatus_t SimStreamUItf::free_buffer(nsuMemory_p fd) {
    if (fd == nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto memory = fdMap[fd];
    fdMap.erase(fd);
    if (memory->self_malloc) {
        free(memory->buffer);
    }
    free(memory);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsuVoidBuf_p SimStreamUItf::get_buffer(nsuMemory_p fd, nsuStreamLen_t length) {
    return fdMap[fd]->buffer;
}

nsukitStatus_t
SimStreamUItf::open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    return I_BaseStreamUItf::open_send(chnl, fd, length, offset);
}

nsukitStatus_t
SimStreamUItf::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    return I_BaseStreamUItf::open_recv(chnl, fd, length, offset);
}

nsukitStatus_t SimStreamUItf::wait_stream(nsuMemory_p fd, float timeout) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t SimStreamUItf::break_stream(nsuMemory_p fd) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t
SimStreamUItf::stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    sim_stream_func(((nsuCharBuf_p )fdMap[fd]->buffer)+offset, length);
//    memset(fdMap[fd]->buffer, 1, length);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t
SimStreamUItf::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                           bool (*stop_event)(), float timeout, int flag) {
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}
