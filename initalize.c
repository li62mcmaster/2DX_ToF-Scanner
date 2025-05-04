//
// Created by Zhenghan Li on 2024-03-31.
//

#include "initalize.h"
#include <stdint.h>
#include "PLL.h"
#include "SysTick.h"
#include "uart.h"
#include "onboardLEDs.h"
#include "tm4c1294ncpdt.h"
#include "VL53L1X_api.h"
#include "initalize.h"


void I2C_Init(void)
{
    SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0;   // Activate I2C0
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // Activate port B

    // Wait until port B is ready
    while ((SYSCTL_PRGPIO_R & 0x0002) == 0) {};

    GPIO_PORTB_AFSEL_R |= 0x0C; // Enable alt function on PB2,3
    GPIO_PORTB_ODR_R |= 0x08;   // Enable open drain on PB3 only
    GPIO_PORTB_DEN_R |= 0x0C;   // Enable digital I/O on PB2,3

    // Configure PB2,3 as I2C
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF00FF) + 0x00002200;

    I2C0_MCR_R = I2C_MCR_MFE;   // Master function enable

    // Configure for 100 kbps clock (added 8 clocks of glitch suppression ~50ns)
    I2C0_MTPR_R = 0b0000000000000101000000000111011;
}


// Initialize onboard LEDs
void onboardLEDs_Init(void)
{
    // Use Port N onboard LEDs
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;            // Activate clock for Port N
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R12) == 0) {}; // Wait for clock to stabilize
    GPIO_PORTN_DIR_R |= 0x03;                            // Make PN0 and PN1 output (built-in LEDs)
    GPIO_PORTN_AFSEL_R &= ~0x03;                         // Disable alternate function on PN0, PN1
    GPIO_PORTN_DEN_R |= 0x03;                            // Enable digital I/O on PN0, PN1
    // Configure PN0, PN1 as GPIO
    // GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R & 0xFFFFFF00); // Not necessary if not changing default PCTL
    GPIO_PORTN_AMSEL_R &= ~0x03;                         // Disable analog functionality on PN0, PN1

    // Use Port F onboard LEDs
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;             // Activate clock for Port F
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0) {}; // Wait for clock to stabilize
    GPIO_PORTF_DIR_R |= 0x11;                            // Make PF0 and PF4 output
    GPIO_PORTF_AFSEL_R &= ~0x11;                         // Disable alternate function on PF4, PF0
    GPIO_PORTF_DEN_R |= 0x11;                            // Enable digital I/O on PF4, PF0
    // Configure PF4, PF0 as GPIO
    // GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R & 0xFFF0FFF0); // Not necessary if not changing default PCTL
    GPIO_PORTF_AMSEL_R &= ~0x11;                         // Disable analog functionality on PF4, PF0

    FlashAllLEDs();
    return;
}

void VL53L1X_XSHUT(void)
{
    GPIO_PORTG_DIR_R |= 0x01;                                        // make PG0 out
    GPIO_PORTG_DATA_R &= 0b11111110;                                 //PG0 = 0
    FlashAllLEDs();
    SysTick_Wait10ms(10);
    GPIO_PORTG_DIR_R &= ~0x01;                                            // make PG0 input (HiZ)
}

// The VL53L1X needs to be reset using XSHUT. We will use PG0
void PortG_Init(void)
{
    // Activate clock for Port G
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R6;                // Activate clock for Port G
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R6) == 0) {};    // Allow time for clock to stabilize
    GPIO_PORTG_DIR_R &= ~0x01;                              // Make PG0 input (HiZ)
    GPIO_PORTG_AFSEL_R &= ~0x01;                            // Disable alt funct on PG0
    GPIO_PORTG_DEN_R |= 0x01;                               // Enable digital I/O on PG0
    // Configure PG0 as GPIO
    // GPIO_PORTG_PCTL_R = (GPIO_PORTG_PCTL_R & 0xFFFFFF00); // Not necessary if not changing default PCTL
    GPIO_PORTG_AMSEL_R &= ~0x01;                            // Disable analog functionality on PG0

    return;
}

void PortK_Init(void)
{
    // Activate the clock for Port K
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R9;                // Activate the clock for Port K
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R9) == 0) {};    // Allow time for clock to stabilize
    GPIO_PORTK_DIR_R = 0xFF;                                // Set all pins of Port K as outputs
    GPIO_PORTK_DEN_R = 0xFF;                                // Enable all pins of Port K as digital pins

    return;
}

void PortJ_Init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;            // Activate clock for Port J
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R8) == 0) {}; // Wait for clock to stabilize
    GPIO_PORTJ_DIR_R &= ~0x03;                           // Make PJ0 and PJ1 input
    GPIO_PORTJ_DEN_R |= 0x03;                            // Enable digital I/O on PJ0 and PJ1
    GPIO_PORTJ_PCTL_R &= ~0x000000FF;                    // Clear PCTL for PJ0 and PJ1
    GPIO_PORTJ_AMSEL_R &= ~0x03;                         // Disable analog functionality on PJ0 and PJ1
    GPIO_PORTJ_PUR_R |= 0x03;                            // Enable weak pull-up on PJ0 and PJ1
}

void PortM_Init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;             // Activate the clock for Port M
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R11) == 0) {}; // Wait for clock to stabilize
    GPIO_PORTM_DIR_R &= ~0x03;                            // Make PM0 and PM1 input
    GPIO_PORTM_DEN_R |= 0x03;                             // Enable digital I/O on PM0 and PM1
    GPIO_PORTM_PCTL_R &= ~0x000000FF;                     // Clear PCTL for PM0 and PM1
    GPIO_PORTM_AMSEL_R &= ~0x03;                          // Disable analog functionality on PM0 and PM1
}


