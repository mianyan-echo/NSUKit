cmake_minimum_required(VERSION 3.26)
project(NSUKit)

set(CMAKE_CXX_STANDARD 17)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

set(ENV{http_proxy} "http://127.0.0.1:10800")
set(ENV{https_proxy} "http://127.0.0.1:10801")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(src)
include_directories(include)
link_directories(lib)

add_library(NSUKit SHARED
#        include/NSUKit.h
        include/xdma_api.h
        include/json/json.h
        include/json/allocator.h
        include/json/assertions.h
        include/json/config.h
        include/json/forwards.h
        include/json/json_features.h
        include/json/reader.h
        include/json/value.h
        include/json/version.h
        include/json/writer.h
        src/lib_json/json_tool.h
        src/lib_json/json_reader.cpp
        src/lib_json/json_value.cpp
        src/lib_json/json_writer.cpp
        src/base_kit.cpp
        src/interface/base_itf.cpp
        src/interface/base_itf.h
        src/middleware/icd_parser.cpp
        src/middleware/icd_parser.h
        src/tools/config.cpp
        src/tools/config.h
        src/interface/pcie_interface.cpp
        src/interface/pcie_interface.h
        src/interface/interface.h
        src/middleware/base_mw.cpp
        src/middleware/base_mw.h
        src/middleware/middleware.h
        src/tools/type.h
        src/middleware/virtual_chnl.cpp
        src/middleware/virtual_chnl.h
        src/tools/type.cpp
        src/interface/tcp_interface.cpp
        src/interface/tcp_interface.h
        src/interface/simulation_interface.cpp
        src/interface/simulation_interface.h)

#target_link_libraries(NSUKit xdma_api)


if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(NSUKit pthread)
endif ()

if (MINGW)
    target_link_libraries(NSUKit ws2_32)
endif ()


# 注册googletest类
enable_testing()
add_executable(
        nsukit_test
        tests/interface_test.cpp
        tests/nsukit_test.cpp
        tests/type_test.cpp
)
target_link_libraries(
        nsukit_test
        gtest_main
)
target_link_libraries(
        nsukit_test
        NSUKit
)

include(GoogleTest)
gtest_discover_tests(nsukit_test)
