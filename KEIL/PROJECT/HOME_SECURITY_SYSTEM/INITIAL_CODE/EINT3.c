#include"EINT3.h"

void ISR_EINT3()  __irq
{
	IOCLR0 = 1<<14;
}

void EINT3_INT()
{
	PINSEL0 |= 0XC0000;
	VICIntSelect =0;

	VICVectCntl0 = 0x20|17;
	VICVectAddr0 = (int)ISR_EINT3;

	EXTMODE = 0X00;
	EXTPOLAR = 0X00;

	VICIntEnable = 1<<17;
}
