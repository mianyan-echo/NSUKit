//
// Created by 56585 on 2023/8/21.
//
#include "test_config.h"

#ifdef TEST_XDMA_LOOP
#include "xdma_api.h"

TEST(XDMA_LOOP_TEST, TEST) {
    const int cmd_board = 0;
    const int xdma_block = 32;    // bytes
    const int loop_num = 1;
    const int valid_addr = 0x00000000;

    fpga_open(cmd_board);
    std::cout << fpga_info_string(cmd_board) << std::endl;
    auto fd = fpga_alloc_dma(cmd_board, xdma_block/4);
    fpga_get_dma_buffer(fd);

    time_t st = 0, et = 0;
    time(&st);
    for(int i=0; i<loop_num; i++) {
        fpga_wr_lite(cmd_board, valid_addr, 1);
        auto _recv_len = fpga_recv(cmd_board, 0, fd, xdma_block/4, 0, 1, 0, 0, DMA_WAIT_FOR_EVER);
        fpga_wr_lite(cmd_board, valid_addr, 0);
        fpga_wr_lite(cmd_board, valid_addr, 1);
        fpga_wr_lite(cmd_board, valid_addr, 0);
        if(_recv_len != xdma_block/4) {
            std::cout << fpga_err_msg() << std::endl;
        }
//        auto _send_len = fpga_send(cmd_board, 0, fd, xdma_block/4, 0, 1, 0, 0, DMA_WAIT_FOR_EVER);
//        if(_send_len != xdma_block/4) {
//            std::cout << fpga_err_msg() << std::endl;
//        }
    }
    time(&et);
    std::cout << "repeat: " << loop_num << " xdma loop test use time: " << et-st << "s" << std::endl;
}

#endif
