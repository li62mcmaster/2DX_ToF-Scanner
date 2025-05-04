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
#include "motor_config.h"


//XSHUT     This pin is an active-low shutdown input;
//					the board pulls it up to VDD to enable the sensor by default.
//					Driving this pin low puts the sensor into hardware standby. This input is not level-shifted.

bool motorEnabled = false;  //Ture in ON state, False in OFF state
bool masterFlag = false;
bool continueFlag = false;
bool restartFlag = false;

uint16_t	dev = 0x29;			//address of the ToF sensor as an I2C slave peripheral
int status = 0;
int deg = 0;
int reverse = 0;


void InitializeSystem(void)
{
    PLL_Init();
    SysTick_Init();
    onboardLEDs_Init();
    I2C_Init();
    UART_Init();
    PortK_Init();
    PortJ_Init();
    PortM_Init();
    PortJ_Interrupt_Init();
    PortM_Interrupt_Init();
    FlashLED1(2);
    FlashLED2(2);
    FlashLED3(2);
    FlashLED4(2);
}

void InitializeAndConfigureSensor(void)
{
    uint8_t byteData, sensorState = 0;
    uint16_t wordData;
    FlashAllLEDs();
    status = VL53L1X_GetSensorId(dev, &wordData);
    status = VL53L1_RdByte(dev, 0x010F, &byteData); //for model ID (0xEA)
    status = VL53L1_RdByte(dev, 0x0110, &byteData); //for module type (0xCC)
    status = VL53L1_RdWord(dev, 0x010F, &wordData); //for both model ID and type
    sprintf(printf_buffer,"(Model_ID, Module_Type)=0x%x\r\n",wordData);
    UART_printf(printf_buffer);
    while (sensorState == 0) {
        status = VL53L1X_BootState(dev, &sensorState);
        SysTick_Wait10ms(10);
    }
    UART_printf("ToF Chip Booted!\r\n Please Wait...\r\n");
    status = VL53L1X_ClearInterrupt(dev);
    status = VL53L1X_SensorInit(dev);
    Status_Check("SensorInit", status);
    status = VL53L1X_SetDistanceMode(dev, 2); // 1=short, 2=long
    status = VL53L1X_StartRanging(dev); // Start Ranging
}

void WaitForInput(char expectedInput) {
    int input;
    do
    {
        input = UART_InChar();
    } while (input != expectedInput);
}

void spinReverse()
{
    GPIO_PORTK_DATA_R = 0b00000001;
    SysTick_Wait10ms(1);
    GPIO_PORTK_DATA_R = 0b00001000;
    SysTick_Wait10ms(1);
    GPIO_PORTK_DATA_R = 0b00000100;
    SysTick_Wait10ms(1);
    GPIO_PORTK_DATA_R = 0b00000010;
    SysTick_Wait10ms(1);
}

void MotorSpinForScanning(int totalScans)
{
    while (motorEnabled != true)
    {
        SysTick_Wait10ms(5);
        FlashAllLEDs();
        if (motorEnabled == true)
        {
            break;
        }
    }
    int degreePerScan = 2048 / totalScans;
    int degree = 0;
    while (degree < degreePerScan)
    {
        if (motorEnabled == false || masterFlag == false)
        {
            break;
        }
        spin();
        degree += 4;
        deg +=4;
        reverse +=4;
    }
    if (deg >= 2048)
    {
        deg = 0;
        motorEnabled = !motorEnabled;
    }
}


void ReadAndSendDistance(uint16_t* Distance) {
    uint16_t RangeStatus, SignalRate, AmbientRate, SpadNum;
    //status = VL53L1X_GetRangeStatus(dev, &RangeStatus);
    //status = VL53L1X_GetSignalRate(dev, &SignalRate);
    //status = VL53L1X_GetAmbientRate(dev, &AmbientRate);
    //status = VL53L1X_GetSpadNb(dev, &SpadNum);
    status = VL53L1X_GetDistance(dev, Distance); // Get the distance
    // Additional reads if needed
    status = VL53L1X_ClearInterrupt(dev); // Clear interrupt for next reading
    sprintf(printf_buffer, "%u\r\n", *Distance);
    UART_printf(printf_buffer);
    SysTick_Wait10ms(50);
}

void ScaningMode(int scan_times, uint16_t Distance, uint8_t dataReady)
{

    for (int curr = 0; curr < scan_times; curr++)
    {
        while (dataReady == 0)
        {
            if (masterFlag == false || motorEnabled == false)
            {
                break;
            }
            status = VL53L1X_CheckForDataReady(dev, &dataReady);
            //FlashLED3(1);
            VL53L1_WaitMs(dev, 5);
        }
        if (masterFlag == false)
        {
            break;
        }
        dataReady = 0;

        MotorSpinForScanning(scan_times);
        FlashLED4(5);
        ReadAndSendDistance(&Distance);
    }

    motorEnabled = true;
    for (int j = 0; j < reverse; j+=4)
    {
        spinReverse();
    }
    motorEnabled = false;
}


int main(void) {

    InitializeSystem();
    uint8_t myByteArray[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} , i=0;
    uint8_t dataReady;
    uint16_t Distance = 0;

    int input = 0;

    SysTick_Wait10ms(100); // let peripherals boot
    FlashAllLEDs();

    while(1)
    {
        WaitForInput('1');
        UART_printf("Program Begins\r\n");
        UART_printf("A\r\n");

        InitializeAndConfigureSensor();

        UART_printf("How many scans per loop?\r\n");
        int scan_times = 32;
        FlashAllLEDs();

        UART_printf("Press PJ1 to start the the program, Press again to exit\r\n");
        while (masterFlag != true)
        {
            SysTick_Wait10ms(1);
            FlashAllLEDs();
            if (masterFlag == true)
            {
                break;
            }
        }

        while (masterFlag == true)
        {
            UART_printf("Initializing Motor.\r\n");
            SysTick_Wait10ms(10);
            UART_printf("Press PJ0 to start the motor\r\n");
            // Get the Distance Measures scan_times times
            ScaningMode(scan_times, Distance, dataReady);

            UART_printf("Press PM1 to continuing to next scan...?\r\n");
            UART_printf("Press PM0 to stop\r\n");
            while (continueFlag != true || restartFlag != true)
            {
                SysTick_Wait10ms(10);
                FlashLED2(5);
                if (continueFlag == true || restartFlag == true)
                {
                    break;
                }
            }
            if (continueFlag == true)
            {
                UART_printf("Initializing Motor.\r\n");
                reverse = 0;
                SysTick_Wait10ms(10);
                motorEnabled = true;
                continueFlag = false;
                // Get the Distance Measures scan_times times
                continue; // back to the beginning of the loop
            }
            if (restartFlag == true)
            {
                UART_printf("E\r\n");
                masterFlag = false;
                VL53L1X_StopRanging(dev);
            }
            while (1) {}
        }
    }

}
