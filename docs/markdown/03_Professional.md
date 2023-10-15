# 进阶使用

<div style="position: fixed; top: 90%; left: 90%">
<a href="#目录" style="text-decoration: none">返回目录</a>
</div>

<span id="目录"></span>

## 目录
* <a href="#基本结构">基本结构</a>
* <a href="#自闭环仿真">自闭环仿真</a>
* <a href="#数据流交互">数据流交互</a>

---
<span id="基本结构"></span>

## 基本结构

<center>![](introduction_global.png)</center>

1. 本SDK提供的接口主要针对射频/数字组件与cpu的交互，将整个交互流程划分为`请求层`、`处理层`、`协议层`三个部分
   1. **请求层**:
      1. 将组件提供的各种参数的读取和配置，看作对一个寄存器的读写，那么参数名即为`寄存器地址`，参数值为`寄存器值`
      2. 将与组件的数据流交互看作一次次DMA，可以指定使用的dma通道、内存地址、数据大小等进行一次dma
   2. **处理层**: 组件与cpu之间可能会约定各种各样的上层交互协议，那么可以在中间处理这一部分中实现上层协议，并将此协议的最终控制接口转为请求所需的读写寄存器和DMA格式
   3. **协议层**: 对诸如TCP、串口、axi_lite、axi_stream等标准通信协议，进行封装。
2. 基于这三个部分的划分，`协议层`、`处理层`的变更不会影响基于`请求层`开发的任何程序

---

<span id="自闭环仿真"></span>

## 自闭环仿真

@todo SimCmdUItf、SimStreamUItf类正在完善

1. 提供SimCmdUItf、SimStreamUItf两个虚拟协议类，可以模拟与板卡的通信过程，支持写入记忆、数据流仿真等一系列功能，方便 **在没有硬件条件时测试上层程序** 
2. 
   ```cpp
   #include <iostream>
   #include "NSUKit.h"
   
   int main() {
   nsukit::NSUSoc<
   nsukit::SimCmdUItf,
   nsukit::SimCmdUItf,
   nsukit::SimStreamUItf> kit{};
   size_t buf_len = 1024*1024;
   
   nsuInitParam_t param;
   
   param.sim_target = 0;
   // 模拟生成数据函数，数据上行时，会向alloc_buffer中写入从1开始的递增数据
   param.sim_stream_func = [](nsuCharBuf_p buf, nsuStreamLen_t len) {
   for (int i=1; i<len; i++) {
   *(buf+i-1) = (char )i;
   }
   };
   
   kit.link_cmd(&param);
   kit.link_stream(&param);
   
   auto fd = kit.alloc_buffer(buf_len);
   auto data_ptr = kit.get_buffer(fd, buf_len);
   
   // 模拟开启采集要配置的寄存器
   kit.write(0x10000000, 1);
   kit.write(0x10000000, 0);
   
   // 第一个参数指定数据流通道，可为0~3
   kit.stream_recv(0, fd, buf_len, 0);
   
   if (*(uint8_t *)data_ptr == 1) {
   std::cout << "模拟上行数据成功" << std::endl;
   }
   return 0;
   }
   ```

---

<span id="数据流交互"></span>

## 数据流交互

### 同步方式
<center>![](block_stream_show.png)</center>

1. 同步方式数据流交互接口: [NSUKit.stream_send](@ref NSUKit_stream_send)、[NSUKit.stream_recv](@ref NSUKit_stream_recv)
2. 如下示例在host上申请了一片1G的内存，对前1kB写入数据20，将这1kB数据下发到板卡

```cpp
kit.link_stream(&param);

auto fd = kit.alloc_buffer(buf_len);
auto data_ptr = kit.get_buffer(fd, buf_len);

// 模拟开启采集要配置的寄存器
kit.write(0x10000000, 1);
kit.write(0x10000000, 0);

// 第一个参数指定数据流通道，可为0~3
kit.stream_recv(0, fd, buf_len, 0);
```

### 异步方式
<center>![](async_stream_itf.png)</center>

1. 提供异步数据流交互接口: [NSUKit.open_send](@ref NSUKit_open_send)、[NSUKit.open_recv](@ref NSUKit_open_recv)、[NSUKit.wait_stream](@ref NSUKit_wait_stream)、[NSUKit.break_stream](@ref NSUKit_break_stream)
2. 接口在开启数据流后立即返回，不等待说有数据传输完成，用户可以在数据流传输过程中继续执行其它操作

```cpp
#include <iostream>
#include <fstream>
#include "NSUKit.h"


int main(int argc, char *argv[]) {
   const int ds_chnl = 0;
std::ofstream outf;
nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> kit{};
nsuInitParam_t param;

std::cout << "当前示例" << argv[0]
     << "为展示基于PCIE的数据上行的接口调用示例，功能为从板卡0的数传通道0上行指定数量的数据并写入文件" << std::endl;
if (argc != 4) {
   std::cout << "不受支持的传参方法" << std::endl;
std::cout << argv[0] << " {IP} {totalBytes} {filePath}" << std::endl;
return 1;
}
nsuSize_t total_len = std::atoi(argv[2]);
if (total_len % 4 != 0) {
std::cout << "上行数据总长度total_len " << total_len << "Bytes应为 " << 4 << "Bytes的整倍数" << std::endl;
return 1;
}

// 连接板卡
param.cmd_ip = argv[1];
param.cmd_board = 0;
param.stream_board = 0;
auto res = kit.link_cmd(&param);
if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
std::cout << "建立CS、CR连接：" << nsukit::status2_string(res) << std::endl;
}
res = kit.link_stream(&param);
if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
std::cout << "建立DS连接：" << nsukit::status2_string(res) << std::endl;
}

// 准备DS连接要用的内存
nsuMemory_p mem = kit.alloc_buffer(total_len);                 // 申请内存
auto ds_buf = (char *)kit.get_buffer(mem, total_len);          // 获取内存首指针

                                                                  // 通知FPGA开始采集
res = kit.execute("系统开启");
if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
std::cout << "系统开启：" << nsukit::status2_string(res) << std::endl;
}

// 上行一个颗粒度的数据
res = kit.open_recv(ds_chnl, mem, total_len, 0);
if (res == nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
std::cout << "成功开启DS交互" << std::endl;
auto ds_state = nsukitStatus_t::NSUKIT_STATUS_STREAM_RUNNING;
while (ds_state != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
ds_state = kit.wait_stream(mem, 1000.);
//            std::cout << "wait_stream：" << nsukit::status2_string(ds_state) << std::endl;
}
}

std::cout << *(uint32_t *)ds_buf << std::endl;

// 通知FPGA开始采集
res = kit.execute("系统停止");
if (res != nsukitStatus_t::NSUKIT_STATUS_SUCCESS) {
std::cout << "系统停止：" << nsukit::status2_string(res) << std::endl;
}

// 存盘
outf.open(argv[3], std::ofstream::binary);
outf.write(ds_buf, total_len);
outf.close();

kit.free_buffer(mem);  // 释放内存
return 0;
}

```

---


<center>Copyright © 2023 耐数 <a href="http://naishu.tech/" target="_blank">naishu.tech</a></center>
<center>北京耐数电子有限公司</center>