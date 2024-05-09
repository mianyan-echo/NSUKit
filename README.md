## 编译方法
```shell
cd build
cmake -DENABLE_TEST=OFF -DNSUKIT_WITH_XDMA=ON -DNSUKIT_DEBUG_EN=OFF ..
cmake --build . --target NSUKit
```

## 引入方法
### gcc
```shell
cp -r NSUKit/include ./
cp NSUKit/lib/libNSUKit.so ./
gcc xxx.cpp -INSUKit/include -lNSUKit
```

## 最简模拟数据上行用例
- **每个例化好的nsukit::NSUSoc类，可以对应一张数据进机卡，对于192单元项目每个板卡有四个数据流通道可用**
- 更多示例可参考examples中的代码
```c++
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