cmake_minimum_required(VERSION 3.26)

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

include_directories(../include)
enable_testing()
set(NSUKitTest_SRC
        interface_test.cpp
        nsukit_test.cpp
        type_test.cpp
        namespace_test.cpp
        xdma_loop_test.cpp
        test_config.h
)

add_executable(nsukit_test ${NSUKitTest_SRC})

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

function(enable_nuskit_test)

endfunction()