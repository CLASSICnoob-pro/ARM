#include"GSM.h"

void GSM_SMS(u8* MSG)
{
	UART0_STR("AT\r\n");
	delay_milli(10);
	UART0_STR("AT+CMGF=1\r\n");
	delay_milli(10);
	UART0_STR("AT+CMGF=");
	UART0_TX('"');
	UART0_STR(NUM);
	UART0_TX('"');
	UART0_STR("\r\n");
	delay_milli(10);
	UART0_STR(MSG);
	UART0_TX(26);
	delay_milli(10);
}

void GSM_OTP(u8* otp)
{
	u32 num;
	int i=0;
	srand(T1PC);
	num = rand()%(9999-1000)+1000;
//	srand(T1PC+otp);
//	otp = rand()%(9999-1000)+1000;
//	return�otp;
    
//    if(num==0)
//        LCD_DATA('0');
//    else
//    {
//        if(num<0)
//        {
//            LCD_DATA('-');
//            num=-num;
//        }
//        
        while( (num>0) && (i<4) )
		{
			otp[i]= (num%10)+48;
			num/=10;
			i++;
	    }
	    
    }

