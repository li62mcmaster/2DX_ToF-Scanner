//
// Created by Zhenghan Li on 2024-03-31.
//

#ifndef INC_2DX_2_1_INITALIZE_H
#define INC_2DX_2_1_INITALIZE_H

#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable

#define MAXRETRIES              5           // number of receive attempts before giving up

void I2C_Init(void);
void PortG_Init(void);
void PortK_Init(void);
void PortJ_Init(void);
void PortM_Init(void);
void onboardLEDs_Init(void);

#endif //INC_2DX_2_1_INITALIZE_H
