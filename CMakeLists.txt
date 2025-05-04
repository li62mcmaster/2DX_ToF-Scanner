cmake_minimum_required(VERSION 3.27)
project(2dx_2_1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(RTE/_Target_1)

add_executable(2dx_2_1
        RTE/_Target_1/RTE_Components.h
        RTE/Device/MSP432E401Y/system_msp432e401y.c
        2dx-2.1.c
        onboardLEDs.c
        onboardLEDs.h
        PLL.c
        PLL.h
        SysTick.c
        SysTick.h
        tm4c1294ncpdt.h
        vl53l1_platform.c
        vl53l1_platform.h
        vl53l1_platform_2dx4.c
        vl53l1_platform_2dx4.h
        vl53l1_types.h
        vl53l1_types_2dx4.h
        VL53L1X_api.c
        VL53L1X_api.h
        uart.h
        uart.c
        initalize.c
        initalize.c
        initalize.h
        interrupt_config.h
        interrupt_config.c
        interrupt_config.c
        motor_config.h
        motor_config.c)
