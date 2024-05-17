#include"LPC21XX.H"
#include"types.h"
#include"I2C.h"
#include"I2C_EEPROM.h"
#include"LCD.h"
#include"UART0.h"
#include"GSM.h"
#include"Whole_Keypad.h"
#include"string.h"
#include"stdio.h"
#include"l293d.h"
#include"EINT3.H" 

extern u8 str_key[100];
extern u8 num_key[20];
u8 otp[4];


int pass_fail = 0,otp_fail = 0,init_val,admin_delay = 3000;
u8 str[100],i;

void USER_LOGIN()
{
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR("ENTER PASSWORD:");
	ALPHA_KEY(0XC0);
	I2C_EEPROM_PAGE_READ(0X50,0X00,str,8);
	UART0_STR(str);
	UART0_STR(str_key);
	if( (strcmp((const char*)str,(const char*)str_key) == 0) )
	{
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		GSM_OTP(otp);
		sprintf((char*)str,"The OPT is %s",otp);
		GSM_SMS(str);
		LCD_STR("Enter OPT:");
		NUM_KEY(0xc0);
		if(0)
		{
			otp:
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			LCD_STR("Renter OPT:");
			NUM_KEY(0xc0);	
		}
		delay_milli(500);
		if( (strcmp((const char*)otp,(const char*)num_key) == 0) )
		{
			LCD_COMMAND(0X01);
		 	LCD_COMMAND(0X80);
			LCD_STR("Door Opening");
			DOOR_OPEN();
			delay_milli(2500);
			LCD_COMMAND(0X01);
		 	LCD_COMMAND(0X80);
			LCD_STR("Door Closing");
			DOOR_CLOSE();
			delay_milli(2500);
			DOOR_IDLE();
			LCD_COMMAND(0X01);	
		}	
		else{
			LCD_COMMAND(0X01);
		 	LCD_COMMAND(0X80);
			LCD_STR("INCORRECT OTP!!!");
			delay_milli(2500);
			goto otp;
		}			 
	}
	else
	{
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		LCD_STR("   INCORRECT");
		LCD_COMMAND(0XC0);
		LCD_STR("  PASSWORD!!!");
		delay_milli(1000);
		LCD_COMMAND(0X01);
	}
} 

void ADMIN_LOGIN()
{
	LCD_COMMAND(0X01);
 	LCD_COMMAND(0X80);
	LCD_STR("ADMIN LOGIN?");
	LCD_COMMAND(0XC0);
	LCD_STR("(Y/N)->");
	ALPHA_KEY(0XC7);
	if( (strcmp("y",(const char*)str_key) == 0) )
	{
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		LCD_STR("ADMIN PASSWORD:");
		ALPHA_KEY(0XC0);
		I2C_EEPROM_PAGE_READ(0X50,0X00,str,8);
		UART0_STR(str_key);
		if( (strcmp((const char*)str,(const char*)str_key) == 0) )
		{
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			GSM_OTP(otp);
			sprintf((char*)str,"The OPT is %s",otp);
			GSM_SMS(str);
			LCD_STR("Enter OPT:");
			NUM_KEY(0xc0);
			if(0)
			{
				otp:
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR("Renter OPT:");
				NUM_KEY(0xc0);	
			}
			delay_milli(500);
			if( (strcmp((const char*)otp,(const char*)num_key) == 0) )
			{
				LCD_COMMAND(0X01);
			 	LCD_COMMAND(0X80);
				LCD_STR("Door Opening");
				DOOR_OPEN();
				delay_milli(2500);
				LCD_COMMAND(0X01);
			 	LCD_COMMAND(0X80);
				LCD_STR("Door Closing");
				DOOR_CLOSE();
				delay_milli(2500);
				DOOR_IDLE();
				LCD_COMMAND(0X01);	
			}	
			else{
				LCD_COMMAND(0X01);
			 	LCD_COMMAND(0X80);
				LCD_STR("INCORRECT OTP!!!");
				delay_milli(2500);
				goto otp;
			}			 
		}
		else
		{
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			LCD_STR("   INCORRECT");
			LCD_COMMAND(0XC0);
			LCD_STR("  PASSWORD!!!");
			delay_milli(1000);
			LCD_COMMAND(0X01);
		}
	}
	delay_milli(2500);
}

int main()
{
	KEYPAD_INIT();
	LCD_INIT();
	I2C_INIT();
	UART0_INIT();
	L293D_INIT();
	DOOR_IDLE();
	for(i=0;i<100;i++)
		str[i]='\0';	
	I2C_EEPROM_PAGE_WRITE(0X50,0X00,"classics",8);
	delay_milli(100);
	I2C_EEPROM_PAGE_READ(0X50,0X00,str,8);
	UART0_STR(str);
//	if( (strcmp((const char*)str,"\0\0\0\0\0") == 0) )
//		 UART0_STR("\r\nyup\r\n");
	if( (str[0] == 0xff) )
		 UART0_STR("\r\nyup\r\n");
	else
	UART0_STR("\r\nnope\r\n");

	while(1)
	{
		for(i=0;i<100;i++)
		str[i]='\0';
//		LCD_COMMAND(0X01);
		//LCD_SCROLL("MULTI-LEVEL SECURITY SYSTEM",0x80,0x8f);
		LCD_COMMAND(0X80);
		LCD_STR("  MULTI-LEVEL");
		LCD_COMMAND(0XC0);
		LCD_STR("SECURITY SYSTEM");
		if(main_sw != 0x07)
		{
			init_val = millis();
			while(main_sw != 0x07);
			if((millis()-init_val) >= admin_delay)
				ADMIN_LOGIN();
			else
				USER_LOGIN();	  				
		}
	}						   
}
