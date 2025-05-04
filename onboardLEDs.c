#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"
#include "onboardLEDs.h"
#include "initalize.h"

#define DELAY 1

//Flash D1
void FlashLED1(int count) {
    /*
     * This function will flash the onboard LED D1 (PN1)
     */
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000010; 								//hello world!
			SysTick_Wait10ms(DELAY);														//.05s delay
			GPIO_PORTN_DATA_R ^= 0b00000010;			
			SysTick_Wait10ms(DELAY);														//.05s delay
		}
}

//Flash D2
void FlashLED2(int count) {  //PN0
    /*
     * This function will flash the onboard LED D2 (PN0)
     */
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000001; 								//hello world!
			SysTick_Wait10ms(DELAY);														//.05s delay
			GPIO_PORTN_DATA_R ^= 0b00000001;			
			SysTick_Wait10ms(DELAY);														//.05s delay			
		}
}

//Flash D3
void FlashLED3(int count) {
    /*
     * This function will flash the onboard LED D3 (PF4)
     */
		while(count--) {
			GPIO_PORTF_DATA_R ^= 0b00010000; 								//hello world!
			SysTick_Wait10ms(DELAY);														//.05s delay
			GPIO_PORTF_DATA_R ^= 0b00010000;			
			SysTick_Wait10ms(DELAY);														//.05s delay			
		}
}

//Flash D4
void FlashLED4(int count) {
    /*
     * This function will flash the onboard LED D4 (PF0)
     */
		while(count--) {
			GPIO_PORTF_DATA_R ^= 0b00000001; 								//hello world!
			SysTick_Wait10ms(DELAY);														//.05s delay
			GPIO_PORTF_DATA_R ^= 0b00000001;			
			SysTick_Wait10ms(DELAY);														//.05s delay			
		}
}

void FlashAllLEDs(){
		GPIO_PORTN_DATA_R ^= 0b00000011; 								//hello world!
		GPIO_PORTF_DATA_R ^= 0b00010001; 								//hello world!	
		SysTick_Wait10ms(25);														//.25s delay
		GPIO_PORTN_DATA_R ^= 0b00000011;			
		GPIO_PORTF_DATA_R ^= 0b00010001; 								//goodbye world!	
		SysTick_Wait10ms(25);														//.25s delay			
}


void FlashI2CTx(){
//	FlashLED1(1);
}

void FlashI2CRx(){
//	FlashLED2(1);
}


//Flash Error D1&D2&D3&D4
void FlashI2CError(int count) {
//		while(count--) {
//			FlashAllLEDs();
//		}
}
