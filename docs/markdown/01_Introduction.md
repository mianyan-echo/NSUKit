# 简介

<div style="position: fixed; top: 90%; left: 90%">
<a href="#目录" style="text-decoration: none; display: none">返回目录</a>
</div>

NSUKit为一套用于各种射频/数字组件的SDK。支持c++、python等编程语言，此接口完成了 **主机对板级交互的层级抽象** ，目标是 **分割上层应用程序和多种多样的板级IO接口** ，可以实现 **一次开发、多Soc兼容** 。

_本文档为NSUKit的C++语言版本的使用文档。_

<center>![](main_framework.png)</center>
<center>NSUKit基本框架</center>

---
## 用户手册
1. [快速开始](02_Quickstart.md)：接口功能详细介绍、安装与基本使用
2. [进阶使用](03_Professional.md)：Soc模拟功能与异步数据流上行接口
3. [ICD指令](04_ICDScheme.md)：介绍了一种主机与板上/片上处理核间的指令协议，nsukit内置支持
4. 基础数据类型定义： include/base/type.h
5. 使用示例：
   1. 指令操作 examples/soc_config.cpp
   2. 固件更新 examples/firmware_update.cpp
   3. DS操作示例 examples/DSOpUseCase.cpp
   4. 异步多线程上行示例 examples/data_upload.cpp
   5. 异步多线程下行递增数示例 examples/data_download.cpp
   6. 多Soc同步 examples/hardware_sync.cpp

---
## 应用示例
1. [使用NSUKit与NRD FDK交互](app/nrd_fdk.md)：提供了对于NSUFDK套件的应用示例，NSUFDK用户可以从此文档开始阅读(待完善)
2. [Oscilloscope mode](app/OscilloscopeMode.md): 配合相应的硬件，此接口可以实现类似示波器的触发采集、触发电平调整、采集时宽配置、多通道数据获取等功能

---
