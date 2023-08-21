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
include_directories(deps/jsoncpp)
link_directories(lib)

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

add_library(NSUKit SHARED ${libNSUKit_SRC})
SET_TARGET_PROPERTIES(NSUKit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../lib)
SET_TARGET_PROPERTIES(NSUKit PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)

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
        tests/namespace_test.cpp
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
