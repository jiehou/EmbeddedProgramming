/*
* GPIO Demo
*/ 
// 1) Make PE1 an output and PE0 an input
// 2) Make LED on initially
// 3) Delay about 100ms
// 4) If the switch is pressed, then toggle LED. Otherwise make LED on
// 5) Repeat 3),4) 
#include "PLL.h"
#include "SysTick.h"

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DATA_R 	    (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define PE0                     (*((volatile unsigned long *)0x40024004))

void PortE_Init(void){ 
	volatile unsigned long delay;
	// 1) activate clock for Port E
  SYSCTL_RCGC2_R |= 0x00000010;
  // allow time for clock to start	
  delay = SYSCTL_RCGC2_R;
  // 2) disable analog on PE1-0
  GPIO_PORTE_AMSEL_R &= ~0x03;
	// 3) PCTL GPIO on PE1-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF;
	// 4) PE0 in, PE1 out (1: Output; 0: Input)
  GPIO_PORTE_DIR_R |= 0x02;
	GPIO_PORTE_DIR_R &= ~0x01;
	// 5) disable alternate function on PE1-0
  GPIO_PORTE_AFSEL_R &= ~0x03;
	// 6) enable digital I/O on PE1-0
  GPIO_PORTE_DEN_R |= 0x03;
}

int main(void){ 	
	PLL_Init_50MHz();
	SysTick_Init();
	PortE_Init();
	GPIO_PORTE_DATA_R = 0x02;	
  while(1){
		// delay 100 ms 
		SysTick_Wait10ms(10);
    if (PE0){
			GPIO_PORTE_DATA_R ^= 0x02;
		}
		else{
			GPIO_PORTE_DATA_R = 0x02;
		}
  }
}
