#include<lpc21xx.h>
#include<delay.h>
#include"uart0.h"

#define CONFIG_BIT  0X00010504
#define PDN_BIT 	21
#define START_BIT	24
#define DONE_BIT	31
#define ADC_READ	((ADDR>>6)&0X3FF)

int main()
{
	float READ_VAL=0;
	UART0_INIT();
	PINSEL1 |= 0X04000000;
	ADCR = CONFIG_BIT;
	ADCR |= (1<<PDN_BIT)|(1<<START_BIT);

	while(1)
	{
		UART0_STR("READ_VAL = ");

		while( ((ADDR>>DONE_BIT)&1) == 0);
		READ_VAL = ADC_READ;
		
		//READ_VAL = 3.3*READ_VAL/1024;

		UART0_INTEGER(READ_VAL);
		UART0_STR("\r\n");

		delay_milli(500);
	}
}

