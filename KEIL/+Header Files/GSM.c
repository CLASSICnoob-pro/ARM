#include"GSM.h"
char NUM[15]= "+91";
void GSM_SMS(u8* MSG)
{
	UART0_STR("AT\r\n");
	delay_milli(100);
	UART0_STR("AT+CMGF=1\r\n");
	delay_milli(100);
	UART0_STR("AT+CMGS=");
	UART0_TX('"');
	UART0_STR((u8*)NUM);
	UART0_TX('"');
	UART0_STR("\r\n");
	delay_milli(100);

	UART0_STR(MSG);
	delay_milli(100);
	UART0_TX(26);
	delay_milli(100);
}

void GSM_OTP(u8* otp)
{
	u32 num;
	int i=0;
	srand(T1PC);
	num = rand()%(9999-1000)+1000;       
        while( (num>0) && (i<4) )
		{
			otp[i]= (num%10)+48;
			num/=10;
			i++;
	    }
    }

