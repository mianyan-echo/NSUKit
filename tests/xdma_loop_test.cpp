//
// Created by 56585 on 2023/8/21.
//
#include "test_config.h"

#ifdef TEST_XDMA_LOOP
#include "xdma_api.h"

TEST(XDMA_LOOP_TEST, TEST) {
    const int xdma_board = 0;
    const int xdma_block = 102400;    // bytes
    const int loop_num = 1000;

    fpga_open(xdma_board);
    std::cout << fpga_info_string(xdma_board) << std::endl;
    auto fd = fpga_alloc_dma(xdma_board, xdma_block/4);
    fpga_get_dma_buffer(fd);

    time_t st = 0, et = 0;
    time(&st);
    for(int i=0; i<loop_num; i++) {
        auto _recv_len = fpga_recv(xdma_board, 0, fd, xdma_block/4, 0, 1, 0, 0, DMA_WAIT_FOR_EVER);
        if(_recv_len != xdma_block/4) {
            std::cout << fpga_err_msg() << std::endl;
        }
        auto _send_len = fpga_send(xdma_board, 0, fd, xdma_block/4, 0, 1, 0, 0, DMA_WAIT_FOR_EVER);
        if(_send_len != xdma_block/4) {
            std::cout << fpga_err_msg() << std::endl;
        }
    }
    time(&et);
    std::cout << "repeat: " << loop_num << " xdma loop test use time: " << et-st << "s" << std::endl;
}

#endif
