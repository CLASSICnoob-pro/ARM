#include"UART0.H"

void UART0_INIT(void)
{
	PINSEL0|= 0X05;
	U0LCR = 0X83;
	U0DLL = 97;
	U0LCR = 0X03;
}

void UART0_TX(u8 D)
{
	while(((U0LSR>>5)&1) == 0);
	U0THR = D;
}

u8 UART0_RX(void)
{
	while((U0LSR&1) == 0);
	return U0RBR;
}

void UART0_STR(u8* str)
{
	while(*str)
		UART0_TX(*str++);
}
