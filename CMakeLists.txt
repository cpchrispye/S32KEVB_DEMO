cmake_minimum_required(VERSION 3.15)
project(S32KEVB_DEMO)

set(CMAKE_CXX_STANDARD 14)


add_definitions(-DCPU_S32K146)
include_directories(inc)
include_directories(RTE/Device/S32K146UAxxxLQx)
include_directories(src/drivers/inc)
include_directories(rtos/inc)
include_directories(rtos/src)
include_directories(C:/Keil_v5/ARM/PACK/ARM/CMSIS/5.4.0/CMSIS/Core/Include)
include_directories(C:/Keil_v5/ARM/PACK/Keil/S32_SDK_DFP/1.2.0/platform/devices)

add_executable(S32KEVB_DEMO
        RTE/Device/S32K146UAxxxLQx/system_S32K146.c
        rtos/src/context.c
        rtos/src/scheduler.c
        src/main.cpp
        src/drivers/src/driver_pins.cpp
        src/drivers/inc/driver_pins.h)
