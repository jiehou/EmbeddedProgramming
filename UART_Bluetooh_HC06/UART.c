#include "UART.h"

// Run Mode Clock Gating Control (UART)
#define SYSCTL_RCGCUART_R 	(*((volatile unsigned long *) 0x400FE618))
// Run Mode Clock Gating Control Register 2 (GPIO)
#define SYSCTL_R_RCGC2_R 		(*((volatile unsigned long *) 0x400FE108))
#define UART5_CTL_R 				(*((volatile unsigned long *) 0x40011030))
#define UART5_IBRD_R 				(*((volatile unsigned long *) 0x40011024))
#define UART5_FBRD_R 				(*((volatile unsigned long *) 0x40011028))
#define UART5_LCRH_R 				(*((volatile unsigned long *) 0x4001102C))
#define UART5_FR_R 					(*((volatile unsigned long *) 0x40011018))
#define UART5_DR_R  				(*((volatile unsigned long *) 0x40011000))	
#define UART_FR_RXFE 				0x00000010
#define UART_FR_TXFF				0x00000020
//--- GPIO ---	
#define GPIO_PORTE_AMSEL_R 	(*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R 	(*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_AFSEL_R 	(*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R 		(*((volatile unsigned long *)0x4002451C))  
/*
* U5Rx --> PE4
* U5Tx --> PE5
*/
void UART5_Init(void)
{
	//--- UART ---
	// 1) provide clock to UART5
	SYSCTL_RCGCUART_R |= 0x00000020;
	// 2) provide clock to PortE
	SYSCTL_R_RCGC2_R |= 0x00000010;
	// 3) disable UART5 during initialization
	UART5_CTL_R &= ~0x00000001;
	// 4) write the integer portion to UART5_IBRD_R
	// int(50MHz/(16*9600))) = 325
	UART5_IBRD_R = 325;
	// 5) write the fractional
	UART5_FBRD_R = 33;
	// 6) frame: start, 8 data bits, stop; enable FIFO
	UART5_LCRH_R = 0x0000070;
	// 7) enable transmitter and receiver for UART5
	UART5_CTL_R |= 0x00000300;
	// 8) enable UART5
	UART5_CTL_R |= 0x00000001;
	//--- GPIO ---
	// 9) diable analog function for PE5 and PE4
	GPIO_PORTE_AMSEL_R &= ~0x30;
	// 10) enable alternate functions for PE5 and PE4
	GPIO_PORTE_AFSEL_R |= 0x30;
	// 11) choose UART function for PE5 and PE4 using PCTL register
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & ~0x00FF0000) + 0x00110000;
	// 12) make PE5 and PE4 as digital IO
	GPIO_PORTE_DEN_R |= 0x30;
}

// 0: No input data
// 1: input data available
unsigned char UART5_Available(void)
{
	return (((UART5_FR_R &= UART_FR_RXFE) != 0) ? 0 : 1);
}

unsigned char UART5_ReadChar(void)
{
	while((UART5_FR_R &= UART_FR_RXFE) != 0) {}
	return ((unsigned char)(UART5_DR_R & 0xFF));
}

void UART5_ReadString(char *bufPtr, unsigned short maxLength)
{
	char inputChar;
	unsigned short length = 0;
	inputChar = UART5_ReadChar();
	while (inputChar != CR)
	{
		// in case of backspace
		if (inputChar == BS)
		{
			if (length)
			{
				length--;
				bufPtr--;
				UART5_WriteChar(BS);
			}
		}
		else if (length < maxLength)
		{
			*bufPtr = inputChar;
			length++;
			bufPtr++;
			UART5_WriteChar(inputChar);
		}
		inputChar = UART5_ReadChar();
	}
	*bufPtr = '\0';
}

void UART5_WriteChar(unsigned char data)
{
	while ((UART5_FR_R &= UART_FR_TXFF) != 0) {}
	UART5_DR_R = data;
}

void UART5_WriteString(char * data)
{
	while(*data != '\0')
	{
		UART5_WriteChar(*data);
		data++;
	}
}
