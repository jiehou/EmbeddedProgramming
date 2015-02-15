// U5Rx (receive) connected to PE4
// U5Tx (transmit) connected to PE5
#include "PLL.h"
#include "UART.h"

// PortF (3-1)
#define GPIO_PORTF_DATA_R  			(*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_AMSEL_R 			(*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R   		(*((volatile unsigned long *)0x4002552C))
#define GPIO_PORTF_DIR_R    		(*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R    		(*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AFSEL_R  		(*((volatile unsigned long *)0x40025420))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF 			0x00000020

// Initialize PortF
void PortF_Init(void) {
	volatile unsigned long delay;
	// 1) activate clock for GPIO PortF
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	delay = SYSCTL_RCGC2_R;
	// 2) disable analog function
	GPIO_PORTF_AMSEL_R = 0x00;
	// 3) PCTL 
	GPIO_PORTF_PCTL_R &= 0xFFFF000F;
	// 4) digital enable
	GPIO_PORTF_DEN_R |= 0x0E;
	// 5) direction
	GPIO_PORTF_DIR_R |= 0x0E;
	// 6) alternate function
	GPIO_PORTF_AFSEL_R &= ~0x0E;
}

void OutCRLF(void){
  UART5_WriteChar(CR);
  UART5_WriteChar(LF);
}

int main(void)
{
	//char myString[6];
  PLL_Init_50MHz();
  UART5_Init(); // initialize UART
	PortF_Init();
	GPIO_PORTF_DATA_R = 0x00;
	UART5_WriteString("r - turn Red LED on");
	OutCRLF();
	UART5_WriteString("b - turn Blue LED on");
	OutCRLF();
	UART5_WriteString("g - turn Green LED on");
	OutCRLF();
	UART5_WriteString("Please input a character: ");
	OutCRLF();
	for(;;)
	{
		// UART_WrtieString 
		/*
		UART_WriteString("Please input a string (max length 5): ");
		UART_ReadString(myString, 5);
		UART_WriteString(" OutString= ");
		UART_WriteString(myString);
		OutCRLF();
		*/
		if (UART5_Available())
		{
			unsigned char recvChar = UART5_ReadChar();
			UART5_WriteString("Received Char: ");
			UART5_WriteChar(recvChar);
			OutCRLF();
			switch (recvChar)
			{
				case 'r':
					// red
					GPIO_PORTF_DATA_R = 0x02;
					break;
				case 'b':
					// blue
					GPIO_PORTF_DATA_R = 0x04;
					break;
				case 'g':
					// green
					GPIO_PORTF_DATA_R = 0x08;
					break;
				default:
					GPIO_PORTF_DATA_R = 0x00;
					break;
			}
		}
	}
}
