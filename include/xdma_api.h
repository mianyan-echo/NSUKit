//----------------------------------------------------------
// XDMA common APIs
//
// for Xilinx DMA/Bridge Subsystem for PCI Express v4.1
//
// Note:
// 1. Only support 1 MSI interrupt vector.
// 2. 
//-----------------------------------------------------------
#ifndef XDMA_API_H
#define XDMA_API_H

#define API_VERSION  20231209

//v20231209 : 1. change MAX_NUM_CARDS to 32
//driver 20231206 undef the __LIBXDMA_DEBUG__ based on 20221219
//v20231206 : 1. test the ring mode
//v20231010 : 1. fix the fpga_wait_dma() bug
//v20230817 : 1. fix the build_descriptors() offset checking bug
//v20230601 : 1. os_isr() desn't quit when read() return -1 and errono == 4.
//v20230505 : 1. os_isr() quit when read() return <-1 more than 1000 times.
//v20221219 : 1. add dma_t a magic_word to show if this dma_t is valid
//v20220701 : 1. add user_irq debug information
//v20220601 : 1. fpga_enable_dma_reg_verify() function
//v20220418 : 1. fpga_open add the test_print parameter
//v20211022 : 1. fix dma->channel updates when change chnl
//            2. take double confirm for dma working check
//            3. fpga_break_dma() got 500ms timeout


#ifdef __APPLE__
#define linux
#endif

#ifdef linux
#define MIN_DRV_VERSION 20221219 //linux
#endif

#ifdef UNDER_RTSS
#define MIN_DRV_VERSION 20201124 //rtx
#endif

#ifndef MIN_DRV_VERSION
#define MIN_DRV_VERSION 2020710  //win64
#endif

#ifdef linux

#define _API_CALL
#define DLLEXPORT extern "C"

#else  //win, rtx

#define _API_CALL __stdcall
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

#define CHNL_NUM 4
#define XDMA_INFO_STR_LEN 2047

#ifndef NULL
#define NULL nullptr
#endif

typedef void *HANDLE;

//APIs for information
DLLEXPORT unsigned int
_API_CALL  fpga_info_vendor_id(unsigned int boardn);                        // Device ID
DLLEXPORT unsigned int
_API_CALL  fpga_info_device_id(unsigned int boardn);                        // Vendor ID
DLLEXPORT unsigned int
_API_CALL  fpga_info_bus_num(unsigned int boardn);                          // Bus number
DLLEXPORT unsigned int
_API_CALL  fpga_info_device_num(unsigned int boardn);                       // Device number
DLLEXPORT unsigned int
_API_CALL  fpga_info_function_num(unsigned int boardn);                     // Function number
DLLEXPORT unsigned int
_API_CALL  fpga_info_max_lane_width(unsigned int boardn);                   // Max lane width, 1 2 4 8 16 32
DLLEXPORT unsigned int
_API_CALL  fpga_info_max_link_speed(unsigned int boardn);                   // Max link speed, 1=2.5G, 2=5G, 3=8G
DLLEXPORT unsigned int
_API_CALL  fpga_info_lane_width(unsigned int boardn);                       // Working lane width, 1 2 4 8 16 32
DLLEXPORT unsigned int
_API_CALL  fpga_info_link_speed(unsigned int boardn);                       // Working link speed, 1=2.5G, 2=5G, 3=8G
DLLEXPORT unsigned int
_API_CALL  fpga_info_max_payload_size(unsigned int boardn);                 // Max payload size in bytes
DLLEXPORT unsigned int
_API_CALL  fpga_info_max_readreq_size(unsigned int boardn);                 // Max read request size in bytes
DLLEXPORT unsigned int
_API_CALL  fpga_info_msi_enable(unsigned int boardn);                       // MSI Interrupt enabled
DLLEXPORT unsigned int
_API_CALL  fpga_info_msix_enable(unsigned int boardn);                      // MSIX Interrupt enabled
DLLEXPORT unsigned int
_API_CALL  fpga_info_msi_num(unsigned int boardn);                          // Number of MSIX Interrupt vectors
DLLEXPORT unsigned int
_API_CALL  fpga_info_axis_width(unsigned int boardn);                       // Bit width of AXIS interface
DLLEXPORT unsigned int
_API_CALL  fpga_info_relaxed_ordering(unsigned int boardn);                 // Relaxed ordering
DLLEXPORT unsigned int
_API_CALL  fpga_info_ip_version(unsigned int boardn);                       // IP core version
DLLEXPORT unsigned int
_API_CALL  fpga_info_kernel_version(unsigned int boardn);                   // OS kernel driver version
DLLEXPORT unsigned int
_API_CALL  fpga_info_api_version(unsigned int boardn);                      // API version
DLLEXPORT unsigned int
_API_CALL  fpga_info_bar_num(unsigned int boardn);                          // Number of BARs
DLLEXPORT unsigned int
_API_CALL  fpga_info_bar_reg(unsigned int boardn);                          // BAR index for XDMA control registers
DLLEXPORT unsigned int
_API_CALL  fpga_info_bar_alite(unsigned int boardn);                        // BAR index for AXI Lite
DLLEXPORT unsigned int
_API_CALL  fpga_info_bar_bypass(unsigned int boardn);                       // BAR index for DMA Bypass
DLLEXPORT unsigned int
_API_CALL  fpga_info_chnl_num(unsigned int boardn, int c2h);                // Dma channels count (0-H2C, 1-C2H)
DLLEXPORT unsigned int
_API_CALL  fpga_info_stream(unsigned int boardn, int c2h, int chnl);        // If the dma channel is AXI-Stream
DLLEXPORT char *_API_CALL fpga_info_string(unsigned int boardn);                          // Information String

//Open a board with board index = 0,1,2,...
//poll_interval_ms: 0=interrupt mode; otherwise it's interrupt poll interval in millisecond
//return true if successed.
DLLEXPORT bool _API_CALL fpga_open(unsigned int boardn, unsigned int poll_interval_ms = 0, unsigned int test_print = 0);

//Poll the interrupts
//interval_ms: The polling period in milisecond. 0 = back to normal interrupt mode.
//DLLEXPORT void _API_CALL fpga_set_pollmode(unsigned int boardn, unsigned int interval_ms);

//Close a board and release all related dma objects.
DLLEXPORT void _API_CALL fpga_close(unsigned int boardn);

//Write a 32bit word to a XMDA engine control register
DLLEXPORT void _API_CALL fpga_write_reg32(unsigned int boardn, unsigned int addr8, unsigned int data);

//Read a 32bit word from a XMDA engine control register
DLLEXPORT unsigned int _API_CALL fpga_read_reg32(unsigned int boardn, unsigned int addr8);

//Write a 64bit word to a XMDA engine control register
DLLEXPORT void _API_CALL fpga_write_reg64(unsigned int boardn, unsigned int addr8, unsigned long long data);

//Read a 64bit word from a XMDA engine control register
DLLEXPORT unsigned long long _API_CALL fpga_read_reg64(unsigned int boardn, unsigned int addr8);

//Create a dma object with data buffer length for 32bit words
//if buffer != NULL, driver will use this buffer, otherwise create new buffer.
//  Note: The buffer's start address must be a mulitple of 64.
//if share_buffer is another dma_t, then share the data buffer.
//  Note: if the share_buffer != NULL, the buffer must = NULL.
DLLEXPORT HANDLE
_API_CALL fpga_alloc_dma(unsigned int boardn, unsigned long long len32, void *buffer = NULL, HANDLE share_dma = NULL);
//For C2H dma only. Beter performance potentially.
DLLEXPORT HANDLE _API_CALL fpga_alloc_dma_recv(unsigned int boardn, unsigned long long len32, void *buffer = NULL,
                                               HANDLE share_buffer = NULL);
//For H2C dma only. Beter performance potentially.
DLLEXPORT HANDLE _API_CALL fpga_alloc_dma_send(unsigned int boardn, unsigned long long len32, void *buffer = NULL,
                                               HANDLE share_buffer = NULL);

//Get the user data buffer start address of the dma object
DLLEXPORT void *fpga_get_dma_buffer(HANDLE dma);

//free the dma buffer
DLLEXPORT void _API_CALL fpga_free_dma(HANDLE dma);

//Start a sending (h2c) dma.
//boardn: board index.
//chnl: dma channel. 0..3 (depends on IP)
//dma: the dma object carries the data buffer.
//len32: transfer length in 32bit words.
//offset: data location (offset) in the dma buffer.
//last: if it has a AXIS TLAST at the end of dma. Only for streaming.
//mm_addr: address on the AXI4 MM interface. Only for MM.
//mm_addr_inc: If the AXI4 MM address increases.
//timeout: dma waiting timeout in milliseconds. -1= infinite. 0=no wait.
//return the transfored data length in DWORDs.
//return DMA_START_ERROR for dma start error. use fpga_err_msg() to the get detial error information.
#define DMA_START_ERROR 0xffffffffffffffffULL
#define DMA_NOWAIT 0
#define DMA_WAIT_FOR_EVER 0xffffffff

DLLEXPORT unsigned long long
_API_CALL fpga_send(unsigned int boardn, unsigned int chnl, HANDLE dma, unsigned long long len32,
                    unsigned long long offset = 0,
                    unsigned int last = 1, unsigned int mm_addr = 0, unsigned int mm_addr_inc = 0,
                    unsigned int timeout = DMA_WAIT_FOR_EVER);

//Start a receiving (c2h) dma
//boardn: board index.
//chnl: dma channel. 0..3 (depends on IP)
//dma: the dma object carries the data buffer.
//len32: transfer length in 32bit words.
//offset: data location (offset) in the dma buffer.
//last: if it has a AXIS TLAST at the end of dma. Only for streaming.
//mm_addr: address on the AXI4 MM interface. Only for MM.
//mm_addr_inc: If the AXI4 MM address increases.
//timeout: dma waiting timeout in milliseconds. DMA_WAIT_FOR_EVER= infinite. 0=no wait.
//return the transfored data length in DWORDs.
//return DMA_START_ERROR for dma start error. use fpga_err_msg() to the get detial error information.
DLLEXPORT unsigned long long
_API_CALL fpga_recv(unsigned int boardn, unsigned int chnl, HANDLE dma, unsigned long long len32,
                    unsigned long long offset = 0,
                    unsigned int last = 1, unsigned int mm_addr = 0, unsigned int mm_addr_inc = 0,
                    unsigned int timeout = DMA_WAIT_FOR_EVER);

//make a single recv dma for a dma objects array. this is for windows only.
DLLEXPORT unsigned long long
_API_CALL fpga_recv_multiple(unsigned int boardn, unsigned int chnl, HANDLE *dma_array, unsigned int dma_num,
                             unsigned long long len, unsigned long long offset, unsigned int last, unsigned int mm_addr,
                             unsigned int mm_addr_inc, unsigned int timeout);

//Wait for a dma to complete,
//if the fpga_send/recv() was called with no wait or returned with timeout.
//dma: the dma object.
//timeout: dma waiting timeout in milliseconds. DMA_WAIT_FOR_EVER= infinite. 0=no wait.
//return the transfored data length in DWORDs.
DLLEXPORT unsigned long long _API_CALL fpga_wait_dma(HANDLE dma, unsigned int timeout = DMA_WAIT_FOR_EVER);

//if the last fpga_wait_dma() calling has timeout.
//return 1 for timeout had happend.
DLLEXPORT int _API_CALL fpga_dma_timeout(HANDLE dma);

//get the transfered data length (in Dwords) of the dma
DLLEXPORT unsigned long long _API_CALL fpga_poll_dma(HANDLE dma);

//break the dma and return the transfered length in Dwords.
DLLEXPORT unsigned long long _API_CALL fpga_break_dma(HANDLE dma);

//pause the dma
DLLEXPORT void _API_CALL fpga_pause_dma(HANDLE dma);

//resume the dma
DLLEXPORT void _API_CALL fpga_resume_dma(HANDLE dma);

//Get speed of the last completed dma in MByte/s
DLLEXPORT double fpga_get_dma_speed(HANDLE dma);

//write a 32bit data to the AXI-lite master interface
DLLEXPORT void _API_CALL fpga_wr_lite(unsigned int boardn, unsigned int addr8, unsigned int data);

//read a 32bit data from the AXI-lite master interface
DLLEXPORT unsigned int _API_CALL fpga_rd_lite(unsigned int boardn, unsigned int addr8);

//Wait for a user interrupts (0-15)
//user_irq_num: the interrupt signal index, 0-15.
//timeout: dma waiting timeout in milliseconds. DMA_WAIT_FOR_EVER= infinite. 0=no wait.
//return 0 for success, !=0 for timeout or other error.
DLLEXPORT unsigned int _API_CALL fpga_wait_irq(unsigned int boardn, unsigned int user_irq_num, unsigned int timeout);

DLLEXPORT unsigned long long _API_CALL fpga_get_irq_ns(unsigned int boardn);

//Setup a callback function for a user interrupt.
//func: a C function with a void* parameter, return a int value.
//      The 'func' will be call once when the interrupt arriving.
//      Normally, the 'func' should return 1 and it will be called again at next interrupt.
//      If the 'func' returns 0, means no more callback, the interrupt will be disabled.
//param: a void* being passed to the callback func.
//return 1 for success
DLLEXPORT unsigned int
_API_CALL fpga_set_irq_callback(unsigned int boardn, unsigned int user_irq_num, int (*func)(void *), void *param);

//Remove the callback function for a user interrupt.
DLLEXPORT void _API_CALL fpga_remove_irq_callback(unsigned int boardn, unsigned int user_irq_num);

//return the last error information string.
DLLEXPORT char *_API_CALL fpga_err_msg(void);

//Create a ring buffer with length = len32 Dwords
//The dma is always paused after the creation. Call fpga_ring_resume() to start the dma.
//return the dma object.
DLLEXPORT HANDLE
_API_CALL fpga_ring_create(unsigned int boardn, unsigned int chnl, unsigned int c2h, unsigned long long len32);

//Pause the ring
#define fpga_ring_pause(ring) fpga_pause_dma(ring)

//Resume the ring
DLLEXPORT void _API_CALL fpga_ring_resume(HANDLE ring);

//close the ring and free the resources
DLLEXPORT void _API_CALL fpga_ring_close(HANDLE ring);

//Break the dma and reset the ring.
DLLEXPORT void _API_CALL fpga_ring_reset(HANDLE ring);

//Get a data buffer with specified lenght from the dma ring buffer.
//For H2C dma ring, the data buffer is vacancy for application to fill new data.
//For C2H dma ring, the data buffer is filled by the ealiest dma data for application to read and process.
//If the data buffer is not avialable, the function will wait until timeout when timeout != 0.
//dma: the dma object.
//len32: length of new data in DWORDs.
//timeout: dma waiting timeout in milliseconds. -1= infinite. 0=no wait.
//return: data buffer pointer. NULL = unavialable or timeout.
DLLEXPORT void *_API_CALL fpga_ring_next_buffer(HANDLE ring, unsigned long long len32, int timeout);

//Get occupied data size (Dwords) in buffer
DLLEXPORT unsigned long long _API_CALL fpga_ring_deep(HANDLE ring);

//Print ring status into a string.
DLLEXPORT void _API_CALL fpga_ring_get_info(HANDLE _dma, char *info);

//Read dma channel registers into a string
DLLEXPORT void _API_CALL fpga_debug_dma_regs(unsigned int boardn, unsigned int chnl, unsigned int c2h, char *info);

//Read interrupt registers into a string
DLLEXPORT void _API_CALL fpga_debug_int_regs(unsigned int boardn, char *info);

//Get the decriptors buffer point.
// The desriptor buffer size = dma buffer size / 128
DLLEXPORT char *_API_CALL fpga_debug_get_descs(HANDLE dma);

DLLEXPORT int _API_CALL fpga_debug_test_regs_wr(unsigned int boardn, char *info);

//Let the dma function to verify every dma register value
DLLEXPORT void _API_CALL fpga_enable_dma_reg_verify(unsigned int boardn);


#endif
