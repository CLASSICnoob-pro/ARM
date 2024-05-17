/*
  Watchdog in LPC2148(ARM7)
  http://www.electronicwings.com/arm7/lpc2148-watchdog-timer
*/

#include <lpc214x.h>
#include <stdint.h>

void delay_ms(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    /* loop to generate 1 milisecond delay with Pclk=15MHz */
	}
}

int main(void)
{
	IODIR0 = IODIR0 | 0x00000003; /* P0.0,P0.1 as outputs for LEDs */
	//IO0CLR = 0x00000003; /* LEDs OFF */
	//delay_ms(1000);
	if ( WDMOD & 0x04 )
	{
		IO0CLR = 0x00000003;
		IO0SET = 0x00000002;	/* P0.1 LED ON */
		delay_ms(500);  /* Indicate Watchdog Reset using LED at P0.1 */
		IO0CLR = 0x00000002; /* P0.1 LED OFF */
		delay_ms(500);
	}
	WDTC = 0x0007FFFF; // For 139msec delay
	WDMOD = 0x03;// Watchdog interrupt enable with watchdog reset
	WDFEED = 0xAA;
	WDFEED = 0x55;
	delay_ms(10);
	WDFEED = 0xAA;
	WDFEED = 0x55;
	IO0SET = 0x00000001;	/* P0.0 LED ON */
	delay_ms(500);
//	IO0CLR = 0x00000001;	/* P0.0 LED OFF */
//	delay_ms(100);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//	IO0SET = 0x00000001;
//	delay_ms(1000);
//	IO0CLR = 0x00000001;
//	delay_ms(100);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//	IO0SET = 0x00000001;
//	delay_ms(60);
//	IO0CLR = 0x00000001;
//	delay_ms(60);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//	IO0SET = 0x00000001;
//	delay_ms(65);
//	IO0CLR = 0x00000001;
//	delay_ms(65);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//	IO0SET = 0x00000001;
//	delay_ms(70);
//	IO0CLR = 0x00000001;
//	delay_ms(70);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//
//	IO0SET = 0x00000001;
//	delay_ms(80);
//	IO0CLR = 0x00000001;
//	delay_ms(80);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//
//	IO0SET = 0x00000001;
//	delay_ms(90);
//	IO0CLR = 0x00000001;
//	delay_ms(90);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//
//	IO0SET = 0x00000001;
//	delay_ms(100);
//	IO0CLR = 0x00000001;
//	delay_ms(100);
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
//	return 0;
}
