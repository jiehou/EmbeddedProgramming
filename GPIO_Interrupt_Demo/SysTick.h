#ifndef _SYSTICK_H_
#define _SYSTICK_H_

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);

// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 20 nsec for 50 MHz clock)
void SysTick_Wait(unsigned long delay);

void SysTick_Wait10ms(unsigned long delay);
#endif
