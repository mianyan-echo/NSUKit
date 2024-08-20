//
// Created by 56585 on 2023/8/15.
//

#include "interface/pcie_interface.h"

using namespace nsukit;

static XDMAOperation_t xdmaOp{};


nsukitStatus_t XDMAOperation_t::open_xdma_board(int board) {
    OpLock.lock();
    if (openedBoard.find(board) == openedBoard.end()) {
        openedBoard[board] = 0;
    }
    if (openedBoard[board] == 0) {
        if (!fpga_open(board)) {
            OpLock.unlock();
            return nsukitStatus_t::NSUKIT_STATUS_ACCEPT_FAIL;
        }
    }
    openedBoard[board] += 1;
    OpLock.unlock();
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t XDMAOperation_t::close_xdma_board(int board) {
    OpLock.lock();
    if (openedBoard.find(board) == openedBoard.end()) {
        OpLock.unlock();
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto num = openedBoard[board];
    num -= 1;
    if (num == 0) fpga_close(board);
    openedBoard[board] = num;
    OpLock.unlock();
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


PCIEStreamUItf::PCIEStreamUItf() = default;


unsigned int PCIECmdUItf::irqNum = 15;


nsukitStatus_t PCIECmdUItf::accept(nsuInitParam_t *param) {
    pciBoard = param->cmd_board;
    sentBase = param->cmd_sent_base;
    recvBase = param->cmd_recv_base;
    irqBase = param->cmd_irq_base;
    sentDownBase = param->cmd_sent_down_base;

    return xdmaOp.open_xdma_board(pciBoard);
}


nsukitStatus_t PCIECmdUItf::close() {
    return xdmaOp.close_xdma_board(pciBoard);
}


nsukitStatus_t PCIECmdUItf::set_timeout(float s) {
    pciTimeout = (int )(s*1000.);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t PCIECmdUItf::send_bytes(nsuBytes_t& bytes) {
    return send_bytes(bytes.data(), bytes.size());
}


nsukitStatus_t PCIECmdUItf::send_bytes(nsuCharBuf_p bytes, nsuSize_t length) {
    OpLock.lock();
    onceTimeout = pciTimeout;
    auto st = std::chrono::high_resolution_clock::now();

    auto res = increment_write(sentBase, bytes, length);
    fpga_wr_lite(pciBoard, sentDownBase, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    fpga_wr_lite(pciBoard, sentDownBase, 0);

    auto _use_time = (double )(std::chrono::high_resolution_clock::now()-st).count() * 1e-9;
    if (_use_time < onceTimeout) {
        OpLock.unlock();
        return nsukitStatus_t::NSUKIT_STATUS_TIMEOUT;
    }
    onceTimeout -= _use_time;
    OpLock.unlock();
    return res;
}


nsukitStatus_t PCIECmdUItf::recv_bytes(nsuSize_t size, nsuCharBuf_p buf) {
    if (size <= 0) return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    if (buf == nullptr) return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;

    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    res |= per_recv();
    res |= increment_read(recvBase, size, buf);
    return res;
}


nsukitStatus_t PCIECmdUItf::write(nsuRegAddr_t addr, nsuRegValue_t value) {
    fpga_wr_lite(pciBoard, addr, value);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t PCIECmdUItf::read(nsuRegAddr_t addr, nsuRegValue_t *buf) {
    if (buf== nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto res = fpga_rd_lite(pciBoard, addr);
    memcpy(buf, &res, sizeof(nsuRegValue_t ));
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t PCIECmdUItf::reset_irq() {
    fpga_wr_lite(pciBoard, irqBase, 0x80000000);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    fpga_wr_lite(pciBoard, irqBase, 0);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t PCIECmdUItf::per_recv() {
    unsigned int cnt = 0;
    while (fpga_rd_lite(pciBoard, irqBase) != 0x8000) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        cnt++;
        if (cnt >= 30*1000*10 -1){
            return nsukitStatus_t::NSUKIT_STATUS_TIMEOUT;
        }
    }
//    auto res = fpga_wait_irq(pciBoard, irqNum, static_cast<int>(onceTimeout*1000));
//    if (res != 0) return nsukitStatus_t::NSUKIT_STATUS_TIMEOUT;
    reset_irq();
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}


nsukitStatus_t PCIEStreamUItf::accept(nsuInitParam_t *param) {
    pciBoard = param->stream_board;

    return xdmaOp.open_xdma_board(pciBoard);
}

nsukitStatus_t PCIEStreamUItf::close() {
    return xdmaOp.close_xdma_board(pciBoard);
}

nsukitStatus_t PCIEStreamUItf::set_timeout(float s) {
    return U_Interface::set_timeout(s);
}

nsuMemory_p PCIEStreamUItf::alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf) {
    auto fd = (nsuMemory_p)fpga_alloc_dma(pciBoard, length, buf);
    downloadProcess[fd] = StreamProcess{};
    uploadProcess[fd] = StreamProcess{};
    return fd;
}

nsukitStatus_t PCIEStreamUItf::free_buffer(nsuMemory_p fd) {
    fpga_free_dma(fd);
    downloadProcess.erase(fd);
    uploadProcess.erase(fd);
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsuVoidBuf_p PCIEStreamUItf::get_buffer(nsuMemory_p fd, nsuStreamLen_t length) {
    return fpga_get_dma_buffer(fd);
}

nsukitStatus_t
PCIEStreamUItf::open_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    if ((chnl >= maxChnl) || (length % byteWidth != 0) || (offset % byteWidth != 0)) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    if (downloadProcess.find(fd) == downloadProcess.end()) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto res = fpga_send(pciBoard, chnl, fd, length/byteWidth, offset/byteWidth, 1, 0, 0, DMA_NOWAIT);
    if (res == DMA_START_ERROR) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL;
    }
    downloadProcess[fd].total = length;
    downloadProcess[fd].current = res*byteWidth;
    uploadProcess[fd].total = 0;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t
PCIEStreamUItf::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    if ((chnl >= maxChnl) || (length % byteWidth != 0) || (offset % byteWidth != 0)) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    if (uploadProcess.find(fd) == uploadProcess.end()) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto res = fpga_recv(pciBoard, chnl, fd, length/byteWidth, offset/byteWidth, 1, 0, 0, DMA_NOWAIT);
    if (res == DMA_START_ERROR) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL;
    }
    uploadProcess[fd].total = length;
    uploadProcess[fd].current = res*byteWidth;
    downloadProcess[fd].total = 0;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t PCIEStreamUItf::wait_stream(nsuMemory_p fd, float timeout) {
    StreamProcess *process = nullptr;
    if (uploadProcess[fd].total != 0) process = &uploadProcess[fd];
    if (downloadProcess[fd].total != 0) process = &downloadProcess[fd];
    if (process == nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    auto res = fpga_wait_dma(fd, (uint32_t)(timeout*1000.));
    process->current = res*byteWidth;
    if (process->current != process->total) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
    }
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t PCIEStreamUItf::break_stream(nsuMemory_p fd) {
    StreamProcess *process = nullptr;
    if (uploadProcess[fd].total != 0) process = &uploadProcess[fd];
    if (downloadProcess[fd].total != 0) process = &downloadProcess[fd];
    if (process == nullptr) {
        return nsukitStatus_t::NSUKIT_STATUS_INVALID_VALUE;
    }
    fpga_break_dma(fd);
    process->total = 0;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t
PCIEStreamUItf::stream_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                            bool(*stop_event) (), float timeout, int flag) {
    open_recv(chnl, fd, length, offset);
    if (stop_event == nullptr) {
        stop_event = []() { return false; };
    }
    while (true) {
        if (stop_event()) {
            break_stream(fd);
            return nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL;
        }

        auto res = wait_stream(fd, timeout);

        if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            return res;
        }
    }
}

nsukitStatus_t
PCIEStreamUItf::stream_send(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset,
                            bool(*stop_event) (), float timeout, int flag) {
    open_send(chnl, fd, length, offset);
    if (stop_event == nullptr) {
        stop_event = []() { return false; };
    }
    while (true) {
        if (stop_event()) {
            break_stream(fd);
            return nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL;
        }

        auto res = wait_stream(fd, timeout);

        if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
            return res;
        }
    }
}


PCIERingUItf::PCIERingUItf() = default;

nsukitStatus_t PCIERingUItf::accept(nsuInitParam_t *param) {
    ring_chnl_size = param->ring_chnl_size;
    auto res = PCIEStreamUItf::accept(param);
    DEBUG_PRINT_CLASS(fpga_info_string(pciBoard));
    return res;
}

nsukitStatus_t PCIERingUItf::close() {
    for (auto &t: ringCache) {
        if (t.second != nullptr) fpga_ring_close(t.second);
    }
    for (auto &t: memory_dict) {
        if (t.second != nullptr) free_buffer(t.second);
    }
    memory_index = 0;
    return PCIEStreamUItf::close();
}

nsuMemory_p PCIERingUItf::alloc_buffer(nsuStreamLen_t length, nsuVoidBuf_p buf) {
    auto mem = new Memory{};

    mem->idx = memory_index;
    mem->mem_size = length;
    mem->finish_event.setEvent();

    memory_dict[memory_index] = mem;
    memory_index++;
    DEBUG_PRINT_CLASS(std::to_string(length));
    return mem;
}

nsukitStatus_t PCIERingUItf::free_buffer(nsuMemory_p fd) {
    auto mem = (Memory *)fd;
    if (!mem->finish_event.isSet()) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
    }
    mem->finish_event.resetEvent();
    memory_dict.erase(mem->idx);
    delete mem;
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsuVoidBuf_p PCIERingUItf::get_buffer(nsuMemory_p fd, nsuStreamLen_t length) {
    auto mem = (Memory *)fd;
    {
        std::unique_lock<std::mutex> lock(mem->mtx);
        return mem->memory;
    }
}

nsukitStatus_t
PCIERingUItf::open_recv(nsuChnlNum_t chnl, nsuMemory_p fd, nsuStreamLen_t length, nsuStreamLen_t offset) {
    auto res = nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
    auto mem = (Memory *)fd;
    if (offset != 0) {
        return nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL;
    }
    DEBUG_PRINT_CLASS("chnl="+std::to_string(chnl)+", length="+std::to_string(length));
    {
        std::unique_lock<std::mutex> lock(mem->mtx);
        if (ringCache[chnl] == nullptr) {
            auto ring = fpga_ring_create(pciBoard, chnl, 1, ring_chnl_size[chnl] / byteWidth);
            fpga_ring_resume(ring);
            ringCache[chnl] = ring;
        }
        mem->ring = ringCache[chnl];
        mem->chnl = chnl;
    }

    return res;
}

nsukitStatus_t PCIERingUItf::wait_stream(nsuMemory_p fd, float timeout) {
    auto mem = (Memory *)fd;
    {
        std::unique_lock<std::mutex> lock(mem->mtx);
        DEBUG_PRINT_CLASS(std::to_string(mem->mem_size/byteWidth));
        nsuMemory_p buffer = fpga_ring_next_buffer(mem->ring, mem->mem_size/byteWidth, int (timeout*1000.));
        DEBUG_PRINT_CLASS("00000");
        if (buffer == nullptr) {
//            char err[10240];
//            fpga_ring_get_info(mem->ring, err);
//            DEBUG_PRINT_CLASS(err);
            return nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
        }
        mem->memory = buffer;
    }
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}

nsukitStatus_t PCIERingUItf::break_stream(nsuMemory_p fd) {
    auto mem = (Memory *)fd;
    DEBUG_PRINT_CLASS("00000");
    {
        std::unique_lock<std::mutex> lock(mem->mtx);
        if (mem->ring == nullptr) {
            return nsukitStatus_t::NSUKIT_STATUS_STREAM_FAIL;
        }
        ringCache[mem->chnl] = nullptr;
        fpga_ring_close(mem->ring);
    }
    return nsukitStatus_t::NSUKIT_STATUS_SUCCESS;
}
