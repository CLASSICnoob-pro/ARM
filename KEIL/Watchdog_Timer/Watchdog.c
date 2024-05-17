#include"lpc21XX.h"
#include"delay.h"
#include"lcd.h"
#include"uart0.h" 

int main()
{
	u32 i=0;
	LCD_INIT();
	UART0_INIT();

	WDTC = 0XFFFFFF;
	WDMOD = 0X03;
	WDFEED = 0XAA;
	WDFEED = 0X55;

	LCD_STR("HI...STARTED...");
	UART0_STR("HI...STARTED..\r\n");
	delay_milli(2000);
	LCD_COMMAND(0X01);
	while(++i)
	{
		//LCD_STR("IN..LOOP..");
		LCD_COMMAND(0X80);
		LCD_INTEGER(WDTV);
		LCD_COMMAND(0XC0);
		LCD_INTEGER(i);
		UART0_INTEGER(WDTV);
		UART0_STR("\r\n");
	}
	
}
