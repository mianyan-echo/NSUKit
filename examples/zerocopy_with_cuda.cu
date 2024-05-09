////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024. Naishu
// NSUKit is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan PSL v2.
// You may obtain a copy of Mulan PSL v2 at:
//          http://license.coscl.org.cn/MulanPSL2
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See the Mulan PSL v2 for more details.
////////////////////////////////////////////////////////////////////////////////

//
// Created by jilianyi<jilianyi@naishu.tech> on 2024/5/8.
//


#include <cuda_profiler_api.h>
#include <cuda.h>
#include <cuda_fp16.h>
#include <cuda_runtime.h>
#include "NSUKit.h"


/**
 * algo example
 * @param ptr p_device_mem
 */
__global__ void cu_example_kernel(uint32_t *ptr) {
    unsigned int i = threadIdx.x;
    unsigned int step = blockDim.x;
    unsigned int b = blockIdx.x;

    unsigned int idx = (b*step+i)&0xFFFFFFFF;
    ptr[idx] = idx;
}


/**
 * Convenience function for checking CUDA runtime API results
 * can be wrapped around any runtime API call. No-op in release builds.
 * @param result
 * @return
 */
inline cudaError_t checkCuda(cudaError_t result) {
    if (result != cudaSuccess) {
        fprintf(stderr, "CUDA Runtime Error: %s\n", cudaGetErrorString(result));
        throw std::runtime_error("CUDA Runtime Error");
    }
    return result;
}


int main(int argc, char **argv) {
    nsuMemory_p p_dma_obj;
    void *p_host_mem, *p_gpu_mem, *p_stream_mem;
    nsuSize_t mem_size = 102400;   // 102400 Bytes
    nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};
    nsuInitParam_t param;
    param.stream_board = 0;

    auto res = kit.link_stream(&param);
    if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "建立DS连接：" << nsukit::status2_string(res) << std::endl;
        return 1;
    }

    checkCuda(cudaMallocHost((void **) &p_host_mem, mem_size));                         // alloc host memory with cuda api

    p_dma_obj = kit.alloc_buffer(mem_size, p_host_mem);                                 // use cuda memory as stream memory
    p_stream_mem = kit.get_buffer(p_dma_obj, mem_size);                                 // get stream memory ptr
    checkCuda(cudaHostGetDevicePointer((void **) &p_gpu_mem, (void *) p_host_mem, 0));  // get gpu memory ptr

    std::cout << "host mem_ptr: "      << p_host_mem
              << " \ngpu mem_ptr: "    << p_gpu_mem
              << " \nstream mem_ptr: " << p_stream_mem << std::endl;

    // stream upload
    auto s = kit.open_recv(0, p_dma_obj, mem_size, 0);
    if (s != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
        std::cout << "Establish CS and CR connections: "  << std::endl;
        return 1;
    }
    s = nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
    while (s==nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING) {
        s = kit.wait_stream(p_dma_obj, 1.);
    }

    // run cuda kernel without copy to gpu memory
    cu_example_kernel<<<1, 4>>>((uint32_t *) p_gpu_mem);
    cudaDeviceSynchronize();

    return 0;
}
