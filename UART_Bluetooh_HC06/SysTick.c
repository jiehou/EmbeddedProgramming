#include "SysTick.h"

#define STCTL_R				(*((volatile unsigned long *)0xE000E010))
#define STRELOAD_R		(*((volatile unsigned long *)0xE000E014))
#define STCURRENT_R		(*((volatile unsigned long *)0xE000E018))
#define STCTL_COUNT 	0x00010000

void SysTick_Init(void) {
	// disable SysTick during setup
	STCTL_R = 0;
	// enable SysTick
	// no interrupt and using system clock
	STCTL_R = 0x00000005;
}

/*
* total wait = delay * bus_cycle = delay * (1 / system clock frequency)
*/ 
void SysTick_Wait(unsigned long delay) {
	// writing zero to STCURRENT_R will clear the counter and COUNT flag
	STCURRENT_R = 0;
	STRELOAD_R = delay - 1;
	// wait for count flag
	while ((STCTL_R & 0x00010000) == 0) {}
}

/*
* system clock: 50 MHz
*/ 
void SysTick_Wait10ms(unsigned long delay) {
	unsigned long i;
	for (i = 0; i < delay; i++) {
		// 500000 * 20 ns = 10 ms
		SysTick_Wait(500000);
	}
}
