//
// Created by Zhenghan Li on 2024-03-31.
//

#include "interrupt_config.h"
#include "tm4c1294ncpdt.h"
#include "onboardLEDs.h"
#include "PLL.h"
#include <stdint.h>
#include "SysTick.h"
#include "initalize.h"
#include "stdbool.h"


void EnableInt(void)    // Enable interrupts
{
    __asm("    cpsie   i\n");
}

void DisableInt(void)   // Disable interrupts
{
    __asm("    cpsid   i\n");
}


void WaitForInt(void)   // Low power wait
{
    __asm("    wfi\n");
}

// Global variable visible in Watch window of debugger
// Increments at least once per button press
volatile unsigned long FallingEdges = 0;

void PortJ_Interrupt_Init(void)
{

    FallingEdges = 0;                         // Initialize counter

    GPIO_PORTJ_IS_R &= ~0x03;                 // PJ0 and PJ1 are edge-sensitive
    GPIO_PORTJ_IBE_R &= ~0x03;                // PJ0 and PJ1 are not triggered by both edges
    GPIO_PORTJ_IEV_R &= ~0x03;                // PJ0 and PJ1 are set for falling edge event
    GPIO_PORTJ_ICR_R = 0x03;                  // Clear interrupt flags for PJ0 and PJ1
    GPIO_PORTJ_IM_R |= 0x03;                  // Arm interrupts on PJ0 and PJ1

    NVIC_EN1_R = 0x00080000;                  // Enable interrupt 51 in NVIC (EN1 register)

    NVIC_PRI12_R = (NVIC_PRI12_R & 0x0FFFFFFF) | 0xA0000000; // Set interrupt priority to 5

    EnableInt();                              // Enable global interrupts
}

void PortM_Interrupt_Init(void)
{

    FallingEdges = 0;                         // Initialize counter

    GPIO_PORTM_IS_R &= ~0x03;                 // PM0 and PM1 are edge-sensitive
    GPIO_PORTM_IBE_R &= ~0x03;                // PM0 and PM1 are not triggered by both edges
    GPIO_PORTM_IEV_R &= ~0x03;                // PM0 and PM1 are set for falling edge event
    GPIO_PORTM_ICR_R = 0x03;                  // Clear interrupt flags for PM0 and PM1
    GPIO_PORTM_IM_R |= 0x03;                  // Arm interrupts on PM0 and PM1

    NVIC_EN2_R = 0x00000100;                  // Enable interrupt for Port M in NVIC (EN2 register)

    NVIC_PRI12_R = (NVIC_PRI12_R & 0xF0FFFFFF) | 0x05000000; // Set interrupt priority to 5

    EnableInt();                              // Enable global interrupts
}

void GPIOJ_IRQHandler(void)
{
    // Check if PJ0 triggered the interrupt
    if (GPIO_PORTJ_RIS_R & 0x01)
    {
        FallingEdges++;                       // Increment counter for PJ0
        motorEnabled = !motorEnabled;             // Toggle masterFlag for PJ0
        FlashLED1(5);
        GPIO_PORTJ_ICR_R = 0x01;              // Clear interrupt flag for PJ0
    }

    // Check if PJ1 triggered the interrupt
    if (GPIO_PORTJ_RIS_R & 0x02)
    {
        FallingEdges++;                       // Increment counter for PJ1
        masterFlag = !masterFlag;
        GPIO_PORTJ_ICR_R = 0x02;              // Clear interrupt flag for PJ1
    }
}

void GPIOM_IRQHandler(void) {
    // Check if PM0 triggered the interrupt
    if (GPIO_PORTM_RIS_R & 0x01)
    {
        FallingEdges++;                       // Increment counter for PM0
        restartFlag = !restartFlag;         // Toggle motorEnabled for PM0
        GPIO_PORTM_ICR_R = 0x01;              // Clear interrupt flag for PM0
    }

    // Check if PM1 triggered the interrupt
    if (GPIO_PORTM_RIS_R & 0x02)
    {
        FallingEdges++;                       // Increment counter for PM1
        continueFlag = !continueFlag;                        // Example reaction for PM1
        GPIO_PORTM_ICR_R = 0x02;              // Clear interrupt flag for PM1
    }
}
