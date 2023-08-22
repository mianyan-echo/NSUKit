# 设置要求的CMake版本
cmake_minimum_required(VERSION 3.26)

# 定义项目名称和版本
project(XDMALoopTest VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include_directories(../include)
link_directories(../lib)

SET(XDMALoopTest_SRC
        XDMALoopTest/main.h
        XDMALoopTest/main.cpp)

add_executable(XDMALoopTest ${XDMALoopTest_SRC})
