#include <stdio.h>
#include "SysTick.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"

// Green
#define GPIO_PF3_Data 					(*((volatile unsigned long *)0x40025020))
// Blue
#define GPIO_PF1_Data 					(*((volatile unsigned long *)0x40025008))
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void); 
/*
* Input:
* PF0 --> SW2
* PF4 --> SW1
* Output:
* PF1 --> Red
* PF3 --> Green
*/
void PortF_Init(void) {
	/* GPIO Configuration */
	// 1) enable clock for Port F
	SYSCTL_RCGC2_R |= 0x00000020;
	// 2) unclok PF0
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R |= 0x01; // make PF0 configurable
	// 3) disable analog functions
	GPIO_PORTF_AMSEL_R &= ~0x1B;
	// 4) configure PF4, PF3, PF1, PF0 as GPIO
	GPIO_PORTF_PCTL_R &= ~0x000FF0FF;
	// 5) PF4, PF0: Input; PF3, PF1: Output
	GPIO_PORTF_DIR_R |= 0x0A;
	GPIO_PORTF_DIR_R &= ~0x11;
	// 6) disable alternate functions
	GPIO_PORTF_AFSEL_R &= ~0x1B;
	// 7) enable digital functions
	GPIO_PORTF_DEN_R |= 0x1B;
	// enable pull-up registers for PF4, PF1
	// in this demo falling-edge trigger is assumed
	GPIO_PORTF_PUR_R |= 0x11;
	/* Interrupt Configuration */
	// 8) enable interrupts for PF4, PF0
	GPIO_PORTF_IM_R |= 0x11;
	// 9) enable interrupt for PortF(IRQ 30) at NVIC
	NVIC_EN0_R |= 0x40000000;
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF0FFFFF) | 0x00600000; // here PortF is given to prority 3
	// 10) PF4, PF0: falling-edge trigger
	GPIO_PORTF_IS_R &= ~0x11;
	GPIO_PORTF_IBE_R &= ~0x11;
	GPIO_PORTF_IEV_R &= ~0x11; // falling-edge
	// 11) clear any priority interrupt
	GPIO_PORTF_ICR_R |= 0x11;
	EnableInterrupts();
}

void GPIOPortF_Handler(void) {
	unsigned int i;
	// SW1(PF4) -> Green
	if(GPIO_PORTF_RIS_R & 0x10) {
		GPIO_PORTF_ICR_R |= 0x10;
		for(i = 0; i < 3; i++) {
			GPIO_PF3_Data |= 0x08;
			// Delay 300ms
			SysTick_Wait10ms(30);
			GPIO_PF3_Data &= ~0x08;
			// Delay 300ms
			SysTick_Wait10ms(30);
		}			
	}
	// SW2(PF0) -> Red
	if(GPIO_PORTF_RIS_R & 0x01) {
		GPIO_PORTF_ICR_R |= 0x01;
		for(i = 0; i < 3; i++) {
			GPIO_PF1_Data |= 0x02;
			// Delay 300ms
			SysTick_Wait10ms(30);
			GPIO_PF1_Data &= ~0x02;
			// Delay 300ms
			SysTick_Wait10ms(30);
		}
	}
}

int main(void) {
	// initialize PLL
	PLL_Init_50MHz();
	// initialize 
	SysTick_Init();
	// initialize PortF
	PortF_Init();
	while(1) {
		WaitForInterrupt();
	}
}
