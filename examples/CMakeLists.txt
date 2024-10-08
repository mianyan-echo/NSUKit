# 设置要求的CMake版本
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和版本
project(NSUKitExample VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include_directories(../include)
link_directories(../lib)


add_executable(FirmwareUpdate firmware_update.cpp)
add_executable(SoCConfig soc_config.cpp)
add_executable(DataDownload data_download.cpp)
add_executable(DSOpUseCase DSOpUseCase.cpp)
add_executable(DataUpload data_upload.cpp)
add_executable(HardwareSync hardware_sync.cpp)
add_executable(LinkOnlyEth link_only_eth.cpp)
add_executable(SendIcdCommandTCP send_icd_command_tcp.cpp)

target_link_libraries(
        FirmwareUpdate
        NSUKit
        xdma_api
)

target_link_libraries(
        SoCConfig
        NSUKit
        xdma_api
)

target_link_libraries(
        DataDownload
        NSUKit
        xdma_api
)

target_link_libraries(
        DataUpload
        NSUKit
        xdma_api
)

target_link_libraries(
        HardwareSync
        NSUKit
        xdma_api
)

target_link_libraries(
        DSOpUseCase
        NSUKit
        xdma_api
)

target_link_libraries(LinkOnlyEth
        NSUKit
        xdma_api
)

target_link_libraries(
        SendIcdCommandTCP
        NSUKit
        xdma_api
)


# Linux下链接pthread库
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(DataUpload pthread)
    target_link_libraries(DataDownload pthread)
    target_link_libraries(LinkOnlyEth pthread)
    target_link_libraries(HardwareSync pthread)
endif ()

