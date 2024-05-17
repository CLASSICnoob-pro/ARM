#include<lpc21xx.h>
#include"i2c.h"
#include"i2c_eeprom.h"
#include"uart0.h"

int main()
{
	int i=0,j=0;
	UART0_INIT();
	I2C_INIT();
	for(i=0x50;i<=0x53;i++)
	{
		for(j=0x00;j<=0xff;j++)
		{
			UART0_STR("\r\ni=");
			UART0_INTEGER(i);
			UART0_STR("\tj=");
			UART0_INTEGER(j);
			I2C_EEPROM_WRITE(i,j,0XFF);
			
		}
		UART0_STR("\r\n\r\n\r\nPAGE ==");
		UART0_INTEGER(i);
		UART0_STR("DONE\r\n\r\n\r\n");
	}
}
