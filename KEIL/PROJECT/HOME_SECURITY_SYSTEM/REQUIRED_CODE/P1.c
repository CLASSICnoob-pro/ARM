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

extern u8 str_key[100];
extern u8 num_key[20];
u8 otp[5];
extern char NUM[15];
const char defaut_num[]="+91";

int pwd_reg = 0x00;
int ph_reg = 0x10;

int pwd_fail = 0,otp_fail = 0,init_val,admin_delay = 3000;
u8 str[100],i;

void PH_NO_WRITE_EEPROM(u8 buffAddr,u8* ph_no)
{
	int i,j,last;
	char org[8][8];
	for(i=0;i<8;i++)
	{
		for(j=0;(j<8)&&(ph_no[((i*8)+j)]);j++)
		{
			//UART0_STR("z");UART0_STR("z");
			org[i][j]=ph_no[(i*8)+j];
			//UART0_STR("y");
		}
		if(j!=8)
		break;
	}
	last=j;
	for(j=0;j<=i;j++)
		I2C_EEPROM_PAGE_WRITE(0X50,buffAddr+(j*8),(u8*)org[j],((j==i)?last:8));		
}

void USER_LOGIN()
{
	user_repwd:
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR("ENTER PASSWORD:");
	ALPHA_KEY(0XC0,8);
	I2C_EEPROM_PAGE_READ(0X50,pwd_reg,str,8);
	//UART0_STR(str_key);
	if(0)
	{
		user_pwd:
		pwd_fail++;
		if(pwd_fail<3)
		{
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			LCD_STR("ENTER PASSWORD:");
			ALPHA_KEY(0XC0,8);
		}
		else
		{
			pwd_fail=0;
			delay_milli(500);
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			LCD_STR("MULTIPLE FAILURE");
			LCD_COMMAND(0XC0);
			LCD_STR("ATTEMPTS.EXITING");
			delay_milli(500);
			return;
		}
	}
	if( (strcmp((const char*)str,(const char*)str_key) == 0) )
	{
		pwd_fail=0;
		user_reotp:
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		GSM_OTP(otp);
		sprintf((char *)str,"The OPT is %s",otp);
		GSM_SMS(str);
		LCD_STR("Enter OPT:");
		NUM_KEY(0xc0,4);
		if(0)
		{
			user_otp:
			otp_fail++;
			if(otp_fail<3)
			{
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR("Renter OPT:");
				NUM_KEY(0xc0,4);
			}
			else
			{
				delay_milli(500);
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR("RESEND OTP?");
				LCD_COMMAND(0XC0);
				LCD_STR("(Y/N)->");
				ALPHA_KEY(0XC7,1);
				if( (strcmp("y",(const char*)str_key) == 0) )
				{
					otp_fail=0;
					goto user_reotp;
				}
				delay_milli(500);
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR("EXIT.....?");
				LCD_COMMAND(0XC0);
				LCD_STR("(Y/N)->");
				ALPHA_KEY(0XC7,1);
				if( (strcmp("y",(const char*)str_key) == 0) )
				{
					otp_fail=0;
					return;
				}	
				else
				{
					otp_fail=0;
					pwd_fail=0;
					goto user_repwd;
				}	 
			}	
		}
		delay_milli(500);
		if( (strcmp((const char*)otp,(const char*)num_key) == 0) )
		{
			otp_fail=0;
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
			goto user_otp;
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
		goto user_pwd;
	}
} 

int main()
{
	KEYPAD_INIT();
	LCD_INIT();
	I2C_INIT();
	UART0_INIT();
	L293D_INIT();
	DOOR_IDLE();

	I2C_EEPROM_PAGE_WRITE(0X50,pwd_reg,"classics",8);
	PH_NO_WRITE_EEPROM(ph_reg,"+917987648081");

	while(1)
	{
		for(i=0;i<100;i++)
		str[i]='\0';
		for(i=0;i<5;i++)
		otp[i]='\0';

		LCD_COMMAND(0X80);
		LCD_STR("  MULTI-LEVEL");
		LCD_COMMAND(0XC0);
		LCD_STR("SECURITY SYSTEM");
		if(main_sw != 0x07)
			USER_LOGIN();	  				
		
	}						   
}

