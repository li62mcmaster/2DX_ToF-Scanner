//
// Created by Zhenghan Li on 2024-03-31.
//

#ifndef INC_2DX_2_1_INTERRUPT_CONFIG_H
#define INC_2DX_2_1_INTERRUPT_CONFIG_H

#include "stdbool.h"

extern bool motorEnabled;  //Motor enable flag PM0
extern bool masterFlag; //Master flag for the control the whole system ON/OFF PJ0
extern bool restartFlag; //Flag to restart the current scan loop PJ1
extern bool continueFlag; //Flag to continue the next scan loop with design distance PM1

void EnableInt(void);    // Enable interrupts
void DisableInt(void);   // Disable interrupts
void WaitForInt(void);   // Low power wait
void PortJ_Interrupt_Init(void);
void PortM_Interrupt_Init(void);
void GPIOJ_IRQHandler(void);
void GPIOM_IRQHandler(void);

#endif //INC_2DX_2_1_INTERRUPT_CONFIG_H
