//
// Created by Zhenghan Li on 2024-03-31.
//
#include <stdint.h>
#include "PLL.h"
#include "SysTick.h"
#include "uart.h"
#include "onboardLEDs.h"
#include "tm4c1294ncpdt.h"
#include "VL53L1X_api.h"
#include "initalize.h"
#include "stdbool.h"
#include "interrupt_config.h"

void spin()
{
    if (motorEnabled == true)
    {
        GPIO_PORTK_DATA_R = 0b00000001;
        SysTick_Wait10ms(1);
        GPIO_PORTK_DATA_R = 0b00000010;
        SysTick_Wait10ms(1);
        GPIO_PORTK_DATA_R = 0b00000100;
        SysTick_Wait10ms(1);
        GPIO_PORTK_DATA_R = 0b00001000;
        SysTick_Wait10ms(1);
    }
    else
    {
        GPIO_PORTK_DATA_R = 0b00000000;
    }
}

void spin_clockwise()
{
    if (motorEnabled == true)
    {
        GPIO_PORTK_DATA_R = 0b00000001;
        SysTick_Wait10ms(1);
        GPIO_PORTK_DATA_R = 0b00000010;
        SysTick_Wait10ms(1);
        GPIO_PORTK_DATA_R = 0b00000100;
        SysTick_Wait10ms(1);
        GPIO_PORTK_DATA_R = 0b00001000;
        SysTick_Wait10ms(1);
    }
    else
    {
        GPIO_PORTK_DATA_R = 0b00000000;
    }
}