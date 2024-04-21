# 〽️Oscilloscope mode

1. **简述** ：本文档为CSA-8A模块(下简称模块/业务模块)示波器模式的软件编程文档，根据此文档可以初步应用CSA-8A模块的一下功能
   1. 切换模块ADC采样率
   2. 使用AD触发能力
   3. 以数据流的形式获取多AD采集到的数据

## 1. 设备初始化
### 1.1. 实例化Soc对象，并连接目标Soc
1. 实例化一个NSUSoc类的对象soc， 
   1. 此soc对象维护了本机与模块具体使用了哪三种交互，以及交互所需的上下文 
   2. 在本例中，选择了TCP作为CS交互接口，PCIE作为CR、DS交互接口，关于CS、CR、DS的定义可参考此段描述 [基础接口](#md_基础接口)
2. 建立连接 
   1. nsuInitParam_t 中保存了目标模块的基础信息 
      1. 设置了cs操作所需的目标ip，即为模块的ip 
      2. 设置了cr操作所需的目标pcie id号，单模块时固定为0 
      3. 设置了ds操作所需的目标pcie id号，单模块时固定为0 
   2. 使用 nsukit::NSUSoc::link_cmd 成员函数建立与模块间的cs、cr链接 
   3. 使用 nsukit::NSUSoc::link_stream 成员函数建立与模块间的ds链接

```cpp
#include "NSUKit.h"

nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> soc{};
nsuInitParam_t param;
param.cmd_ip = "xxx.xxx.xxx.xxx";
param.cmd_board = 0;
param.stream_board = 0;
auto res = soc.link_cmd(&param);
res |= soc.link_stream(&param);
```
### 1.2. 初始化Soc
1. 可通过cs交互完成对业务模块的初始化
   1. CS交互接口分为如下三个部分，具体说明可参考 [指令流交互](#md_指令流交互)
      1. nsukit::NSUSoc::get_param  
      2. nsukit::NSUSoc::set_param 
      3. nsukit::NSUSoc::execute
2. 初始化业务模块时，可配置模块的ADC、DAC采样率，是否开启上下变频等
3. 本例中将业务模块的ADC采样率配置为4000Msps，并通过执行`RF配置`指令，将参数配置给业务模块

```cpp
#include "NSUKit.h"
 
nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> soc{};
...

soc.set_param("ADC采样率", 4000);
soc.execute("RF配置");
```

## 2. 触发配置
### 2.1. 配置触发使能及触发电平

1. 将CH0的触发使能打开，将触发电平设置为4096，
2. 将ADC采集到的电平进行int16量化后，所得量化值与此值(4096)做比较，大于此值则被触发
3. 通过 `ADTriggerConfig` 指令下发给设备

```cpp
#include "NSUKit.h"
 
nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> soc{};
...

soc.set_param("CH0TriggerEnable", 1);
soc.set_param("CH0TriggerLevel", 4096);
soc.execute("ADTriggerConfig");
```

### 2.2. 配置预取点数

1. 将8个AD CH的预取点数配置为512
   1. 此值应为16的整倍数，8个通道保持一致
   2. 含义为预取在配置的触发电平`TriggerLevel`到来之前的`PreTriggerPoints`个点
2. 通过 `ADTriggerConfig` 指令下发给设备

```cpp
#include "NSUKit.h"
 
nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> soc{};
...

soc.set_param("PreTriggerPoints", 512);
soc.execute("ADTriggerConfig");
```


## 3. 多通道采集配置

### 3.1. 配置采集总点数

1. 将8个ADC配置为采集128k点，通过 `ADTriggerConfig` 指令下发给设备
2. **注意** ：可在配置完`ADTriggerConfig`指令所携带的多个参数后，统一执行一次`ADTriggerConfig`

```cpp
#include "NSUKit.h"
 
nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> soc{};
...

soc.set_param("TotalTriggerPoints", 128);
soc.execute("ADTriggerConfig");
```

### 3.2. Start/Stop

1. 系统开启，此时如果有CH开启了触发模式，则对应CH每来一次符合电平要求的触发，每个CH都会采集 `TotalTriggerPoints`个点
```cpp
#include "NSUKit.h"
 
nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> soc{};
...

soc.execute("系统开启");
```

2. 系统停止，在此状态下，模块不检测触发，无数据上行
```cpp
#include "NSUKit.h"
 
nsukit::NSUSoc<nsukit::TCPCmdUItf, nsukit::PCIECmdUItf, nsukit::PCIEStreamUItf> soc{};
...

soc.execute("系统停止");
```

## 4. 多通道数据获取

### 4.1. DS交互

1. DS交互流程可参照示例代码
   1. 异步上行 examples/data_upload.cpp
   2. 同步上行 examples/DSOpUseCase.cpp
2. DS交互分为申请DS交互内存、开启DS交互、等待DS交互完成三个流程
   1. 申请到的一片DS交互内存可重复利用，不必在DS交互完成后释放
   2. DS交互的解释可参照此文档: [数据流交互](#md_数据流交互)


### 4.2. 多通道数据交织格式

1. 由于一个业务模块具备8个ADC CH，但模块的DS交互接口只具备一个数据传输通道；所以模块会将8个通道的时域数据按一定规则打包后传输给主机
2. 业务模块的打包规则为
   1. 一个采样点的数据位宽为int16
   2. 每次采集到TotalTriggerPoints个点时，8个CH的原始时域数据前会分别加上一个256个Bytes的包头
   3. 8段带包头的数据会以256个字节为一组，按顺序交织在一起。
   4. 即 `256B CH0` - `256B CH1` - ... - `256B CH7` - `256B CH0` - ...
3. 此打包格式具备独立的可自解释的包头，为追求性能时，可由用户软件直接按固定规则取数，而不解析打包包头

## 5. 算力模块调用(可选)

如业务模块上配备有独立的算力模块，可通过本部分提供的方式调用算力模块算力进行计算

### 5.1. Nvidia AGX GPU

1. [Xavier DataSheet](https://developer.nvidia.com/embedded/downloads#?search=Data%20Sheet&tx=$product,jetson_agx_xavier)
2. [CUDA Doc](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html)
3. 程序结构可参照 examples/data_upload.cpp ，并将write_file_thread改为具体算法
