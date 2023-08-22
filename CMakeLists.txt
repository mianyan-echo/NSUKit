# 设置要求的CMake版本
cmake_minimum_required(VERSION 3.26)

# 定义项目名称和版本
project(NSUKit VERSION 0.1.0)
# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 输出操作系统信息
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

# 设置代理环境变量
set(ENV{http_proxy} "http://127.0.0.1:10800")
set(ENV{https_proxy} "http://127.0.0.1:10801")

# 包含FetchContent模块
include(FetchContent)

# 下载并构建googletest库
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: 防止覆盖父项目的编译器/链接器设置
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# =======================================项目信息配置=============================================
# 配置版本头文件
configure_file(version.h.in ../include/version.h)

# 设置包含目录
include_directories(src)
include_directories(include)
include_directories(deps/jsoncpp)
link_directories(lib)

# 列出项目的源代码文件
SET(libNSUKit_SRC
#        include/NSUKit.h
        include/xdma_api.h
        deps/jsoncpp/json/json.h
        deps/jsoncpp/json/allocator.h
        deps/jsoncpp/json/assertions.h
        deps/jsoncpp/json/config.h
        deps/jsoncpp/json/forwards.h
        deps/jsoncpp/json/json_features.h
        deps/jsoncpp/json/reader.h
        deps/jsoncpp/json/value.h
        deps/jsoncpp/json/version.h
        deps/jsoncpp/json/writer.h
        deps/jsoncpp/lib_json/json_tool.h
        deps/jsoncpp/lib_json/json_reader.cpp
        deps/jsoncpp/lib_json/json_value.cpp
        deps/jsoncpp/lib_json/json_writer.cpp
        src/base_kit.cpp
        src/interface/base_itf.cpp
        src/middleware/icd_parser.cpp
        src/utils/config.cpp
        src/interface/pcie_interface.cpp
        src/middleware/base_mw.cpp
        src/middleware/virtual_chnl.cpp
        src/utils/type.cpp
        src/interface/tcp_interface.cpp
        src/interface/sim_interface.cpp
        src/interface/serial_interface.cpp
        include/interface/base_itf.h
        include/middleware/icd_parser.h
        include/utils/config.h
        include/interface/pcie_interface.h
        include/interface/interface.h
        include/middleware/base_mw.h
        include/middleware/middleware.h
        include/utils/type.h
        include/middleware/virtual_chnl.h
        include/interface/tcp_interface.h
        include/interface/sim_interface.h
        include/interface/serial_interface.h)

# 创建共享库NSUKit
add_library(NSUKit SHARED ${libNSUKit_SRC})
SET_TARGET_PROPERTIES(NSUKit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../lib)
SET_TARGET_PROPERTIES(NSUKit PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)

target_link_libraries(NSUKit xdma_api)
# Linux下链接pthread库
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(NSUKit pthread)
endif ()

# Windows下链接ws2_32库
if (MINGW)
    target_link_libraries(NSUKit ws2_32)
endif ()
# ==============================================================================


# ================================启用测试========================================
enable_testing()
# 添加测试用例
add_executable(
        nsukit_test
        tests/interface_test.cpp
        tests/nsukit_test.cpp
        tests/type_test.cpp
        tests/namespace_test.cpp
        tests/xdma_loop_test.cpp
        tests/test_config.h
)
# 链接测试用例和库
target_link_libraries(
        nsukit_test
        gtest_main
        NSUKit
        xdma_api
)
# 包含Google Test模块
include(GoogleTest)
# 发现并添加测试
gtest_discover_tests(nsukit_test)
# ============================================================================