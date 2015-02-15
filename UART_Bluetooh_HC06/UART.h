// Carriage Return
#define CR 0x0D
// Line Feed
#define LF 0x0A
// Backspace
#define BS 0x08
/*
* Initialize the UART5 with baud rate 9600 (assuming 50 MHz System Clock)
* Frame (total 10 bits): start, 8 data bits, stop
* FIFO enabled and no interrupt
*/
void UART5_Init(void);

/*
* Check whether there are some availabe received characters
* 1: some receive characters are available
* 0: no available received characters
*/
unsigned char UART5_Available(void);

/*
* Read a received character
*/
unsigned char UART5_ReadChar(void);

/*
* Read characters from serial port until ENTER key is typed
* or until max length of the string is reached
*/
void UART5_ReadString(char *bufPtr, unsigned short maxLength);
	
/*
* Write a character
*/
void UART5_WriteChar(unsigned char data);

/*
* Write string
*/
void UART5_WriteString(char* data);
