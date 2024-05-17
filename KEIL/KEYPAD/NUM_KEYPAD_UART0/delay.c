#include"delay.h"

int millis(void)
{
	T1PR = 15000-1;
	T1TCR = 0X01;
	return T1TC;
}

void delay_micro(u32 d)
{
	T0PR = 15000000 - 1;
	T0TCR = 0X01;
	while(T0TC < d);
	T0TCR = 0X03;
	T0TCR = 0X00;
}

void delay_milli(u32 d)
{
	T0PR = 15000 - 1;
	T0TCR = 0X01;
	while(T0TC < d);
	T0TCR = 0X03;
	T0TCR = 0X00;
}

void delay_sec(u32 d)
{
	T0PR = 15 - 1;
	T0TCR = 0X01;
	while(T0TC < d);
	T0TCR = 0X03;
	T0TCR = 0X00;
}
