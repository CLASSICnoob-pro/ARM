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

int DATA_ERROR(u8* s)
{
	int i;
	for(i=0;s[i];i++)
	{
		if(s[i]==0XFF)
		{
			return 1;
		}
	}
	return 0;
}	

void PH_NO_WRITE_EEPROM(u8 buffAddr,u8* ph_no)
{
	int i,j,last;
	char org[8][8];
	for(i=0;i<8;i++)
	{
		for(j=0;(j<8)&&(ph_no[((i*8)+j)]);j++)
		{
			UART0_STR("z");UART0_STR("z");
			org[i][j]=ph_no[(i*8)+j];
			UART0_STR("y");
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
	UART0_STR(str_key);
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

void ADMIN_LOGIN()
{
	LCD_COMMAND(0X01);
 	LCD_COMMAND(0X80);
	LCD_STR("ADMIN LOGIN?");
	LCD_COMMAND(0XC0);
	LCD_STR("(Y/N)->");
	ALPHA_KEY(0XC7,1);
	if( (strcmp("y",(const char*)str_key) == 0) )
	{
		admin_repwd:
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		LCD_STR("ENTER PASSWORD:");
		ALPHA_KEY(0XC0,8);
		I2C_EEPROM_PAGE_READ(0X50,pwd_reg,str,8);
		//UART0_STR(str_key);
		if(0)
		{
			admin_pwd:
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
			admin_reotp:
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			GSM_OTP(otp);
			sprintf((char*)str,"The OPT is %s",otp);
			GSM_SMS(str);
			LCD_STR("Enter OPT:");
			NUM_KEY(0xc0,4);
			if(0)
			{
				admin_otp:
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
						goto admin_reotp;
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
						goto admin_repwd;
					}	 
				}	
			}
			delay_milli(500);
			if( (strcmp((const char*)otp,(const char*)num_key) == 0) )
			{
				admin_change:
				delay_milli(500);
				LCD_COMMAND(0X01);
			 	LCD_COMMAND(0X80);
				LCD_STR("WHICH ONE DO YOU");
			 	LCD_COMMAND(0XC0);
				LCD_STR("WANT TO CHANGE?");
				delay_milli(500);
				LCD_COMMAND(0X01);
			 	LCD_COMMAND(0X80);
				LCD_STR("1. PASSWORD");
			 	LCD_COMMAND(0XC0);
				LCD_STR("2. PHONE NUMBER");
				delay_milli(500);
				LCD_COMMAND(0X01);
			 	LCD_COMMAND(0X80);
				LCD_STR("CHOOSE OPTION :");
			 	LCD_COMMAND(0XC0);
				LCD_STR("1/2/0-EXIT->");
				NUM_KEY(0XCC,1);
				switch(num_key[0])
				{
					case '0':
					{
								delay_milli(500);
								LCD_COMMAND(0X01);
								LCD_COMMAND(0X80);
								LCD_STR("EXIT.....?");
								LCD_COMMAND(0XC0);
								LCD_STR("(Y/N)->");
								ALPHA_KEY(0XC7,1);
								if( (strcmp("y",(const char*)str_key) == 0) )
									return;
								else
									goto admin_change;
								break;
					}

					case '1':
					{
								re1:
								delay_milli(500);
								LCD_COMMAND(0X01);
								LCD_COMMAND(0X80);
								LCD_STR("ENTER PASSWORD:");	
								ALPHA_KEY(0XC0,8);
								UART0_STR("\r\n");
								UART0_STR(str_key);
								strncpy((char*)str,(char*)str_key,9);
								UART0_STR("\r\n");
								UART0_STR(str);
								LCD_COMMAND(0X01);
								LCD_COMMAND(0X80);
								LCD_STR("RE-ENTER PASS:");	
								ALPHA_KEY(0XC0,8);
								UART0_STR("\r\n");
								UART0_STR(str_key);
								delay_milli(500);
								if( (strcmp((const char*)str,(const char*)str_key) == 0) )
								{
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR("PASSWORD SAVED!!");
									LCD_COMMAND(0XC0);
									LCD_STR("********");
									delay_milli(500);
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR(" DO YOU WANT TO");
									LCD_COMMAND(0XC0);
									LCD_STR("CHANGE PASSWORD");
									delay_milli(500);
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR("CONTINUE SETUP");
									LCD_COMMAND(0XC0);
									LCD_STR("(Y/N)->");
									ALPHA_KEY(0XC7,1);
									if( (strcmp("y",(const char*)str_key) == 0) )
									{
										re2:
										I2C_EEPROM_PAGE_WRITE(0X50,pwd_reg,str,8);
										for(i=0;i<100;i++)
										str[i]='\0';
										I2C_EEPROM_PAGE_READ(0X50,pwd_reg,str,8);
										UART0_STR(str);

										delay_milli(500);
										LCD_COMMAND(0X01);
										LCD_COMMAND(0X80);
										LCD_STR("SAVING COMPLETED");
										LCD_COMMAND(0XC0);
										LCD_STR("  PASSWORD SAVED");
										delay_milli(1000);
										
										LCD_COMMAND(0X01);
									}
									else
									{
										delay_milli(500);
										LCD_COMMAND(0X01);
										LCD_COMMAND(0X80);
										LCD_STR("CHANGE PASSWORD?");
										LCD_COMMAND(0XC0);
										LCD_STR("(y/n)->");
										ALPHA_KEY(0XC7,1);
										if( (strcmp("y",(const char*)str_key) == 0) )
											goto re1;
										else
											goto re2;
									}
								}
								else
								{
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR(" PASSWORD DON'T");
									LCD_COMMAND(0XC0);
									LCD_STR("MATCH...RE-ENTER");
									delay_milli(1000);
									LCD_COMMAND(0X01);
									delay_milli(500);
									goto re1;
								}
								break;
					}

					case '2':
					{
								re3:
								LCD_COMMAND(0X01);
								LCD_COMMAND(0X80);
								LCD_STR("ENTER PH. NO.:");	
								NUM_KEY(0XC0,10);
								UART0_STR("\r\n1=");
								UART0_STR(num_key);
								strncpy((char*)str,(char*)num_key,14);
								UART0_STR("\r\ncpy=");
								UART0_STR(str);
								LCD_COMMAND(0X01);
								LCD_COMMAND(0X80);
								LCD_STR("RE-ENTER PH. NO.:");	
								NUM_KEY(0XC0,10);
								UART0_STR("\r\n2=");
								UART0_STR(num_key);
								if( (strcmp((const char*)str,(const char*)num_key) == 0) )
								{
									strcpy((char*)NUM,defaut_num);
									strncat((char*)NUM,(const char*)str,10);
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR("PH. NO. SAVED!!!");
									LCD_COMMAND(0XC0);
									LCD_STR(NUM);
									delay_milli(500);
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR(" DO YOU WANT TO");
									LCD_COMMAND(0XC0);
									LCD_STR("CHANGE PH.NO. OR");
									delay_milli(500);
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR("CONTINUE (Y/N):");
									LCD_COMMAND(0XC0);
									LCD_STR("->");
									ALPHA_KEY(0XC2,1);
									if( (strcmp("y",(const char*)str_key) == 0) )
									{
										re4:
										PH_NO_WRITE_EEPROM(ph_reg,(u8*)NUM);
										for(i=0;i<100;i++)
											str[i]='\0';
										I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
										UART0_STR(str);

										init_reotp:
										delay_milli(500);
										LCD_COMMAND(0X01);
										LCD_COMMAND(0X80);
										LCD_STR("TO VERIFY PH.NO.");
										LCD_COMMAND(0XC0);
										LCD_STR(" SENDING OTP...");
										delay_milli(500);
										LCD_COMMAND(0X01);
										LCD_COMMAND(0X80);
										GSM_OTP(otp);
										sprintf((char*)str,"The OPT is %s",otp);
										GSM_SMS(str);
										LCD_STR("Enter OPT:");
										NUM_KEY(0xc0,4);
										if(0)
										{
											init_otp:
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
												LCD_STR("SAVED PH. NO.");
												LCD_COMMAND(0XC0);
												I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
												UART0_STR(str);
												LCD_STR((char*)str);
												delay_milli(500);
												LCD_COMMAND(0X01);
												LCD_COMMAND(0X80);
												LCD_STR("CHANGE PhNo(y/n)");
												LCD_COMMAND(0XBD);
												LCD_STR((char*)str);
												LCD_STR("->");
												ALPHA_KEY(0XCE,1);
												if( (strcmp("y",(const char*)str_key) == 0) )
													goto re3;
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
														goto init_reotp;
													}	 
												}
												
											}

										}
										delay_milli(500);
										if( (strcmp((const char*)otp,(const char*)num_key) == 0) )
										{
											otp_fail=0;
											delay_milli(500);
											LCD_COMMAND(0X01);
											LCD_COMMAND(0X80);
											LCD_STR("PH.NO. VERIFIED");
											LCD_COMMAND(0XC0);
											I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
											UART0_STR(str);
											LCD_STR((char*)str);

											delay_milli(500);
											LCD_COMMAND(0X01);
											LCD_COMMAND(0X80);
											LCD_STR("SAVING COMPLETED");
											LCD_COMMAND(0XC0);
											LCD_STR("  PH. NO. SAVED");
											delay_milli(1000);
											
											LCD_COMMAND(0X01);	
										}	
										else
										{
											LCD_COMMAND(0X01);
											LCD_COMMAND(0X80);
											LCD_STR("INCORRECT OTP!!!");
											delay_milli(2500);
											goto init_otp;
										}	
									}
									else
									{
										delay_milli(500);
										LCD_COMMAND(0X01);
										LCD_COMMAND(0X80);
										LCD_STR("CHANGE PH. NO. ?");
										LCD_COMMAND(0XC0);
										LCD_STR("(y/n)->");
										ALPHA_KEY(0XC7,1);
										if( (strcmp("y",(const char*)str_key) == 0) )
											goto re3;
										else
											goto re4;
									}			 
								}
								else
								{
									LCD_COMMAND(0X01);
									LCD_COMMAND(0X80);
									LCD_STR("PHONE NO. DON'T");
									LCD_COMMAND(0XC0);
									LCD_STR("MATCH...RE-ENTER");
									delay_milli(1000);
									LCD_COMMAND(0X01);
									delay_milli(500);
									goto re3;
								}
								break;
					}
					default:
					{
								delay_milli(500);
								LCD_COMMAND(0X01);
								LCD_COMMAND(0X80);
								LCD_STR("INVALID OPTION:");
								LCD_COMMAND(0XC0);
								LCD_STR("OPTIONS-> 0/1/2");
								ALPHA_KEY(0XC7,1);
								delay_milli(500);
								goto admin_change;
								break;
					}
				}
				
				LCD_COMMAND(0X01);	
			}	
			else{
				LCD_COMMAND(0X01);
			 	LCD_COMMAND(0X80);
				LCD_STR("INCORRECT OTP!!!");
				delay_milli(2500);
				goto admin_otp;
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
			goto admin_pwd;
		}
	}
	delay_milli(2500);
}

void INIT_SETUP()
{	
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR("    WELCOME");
	LCD_COMMAND(0XC0);
	LCD_STR("       TO");
	delay_milli(500);
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR("  MULTI-LEVEL");
	LCD_COMMAND(0XC0);
	LCD_STR("SECURITY SYSTEM");
	delay_milli(500);
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR("INITIAL");
	LCD_COMMAND(0XC0);
	LCD_STR("SETUP");
	delay_milli(500);
	re1:
	LCD_COMMAND(0X01);
 	LCD_COMMAND(0X80);
	LCD_STR("ENTER PASSWORD:");	
	ALPHA_KEY(0XC0,8);
	strncpy((char*)str,(char*)str_key,9);
	LCD_COMMAND(0X01);
 	LCD_COMMAND(0X80);
	LCD_STR("RE-ENTER PASS:");	
	ALPHA_KEY(0XC0,8);
	delay_milli(500);
	if( (strcmp((const char*)str,(const char*)str_key) == 0) )
	{
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		LCD_STR("PASSWORD SAVED!!");
		LCD_COMMAND(0XC0);
		LCD_STR("********");
		delay_milli(500);
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		LCD_STR(" DO YOU WANT TO");
		LCD_COMMAND(0XC0);
		LCD_STR("CHANGE PASSWORD");
		delay_milli(500);
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		LCD_STR("CONTINUE SETUP");
		LCD_COMMAND(0XC0);
		LCD_STR("(Y/N)->");
		ALPHA_KEY(0XC7,1);
		if( (strcmp("y",(const char*)str_key) == 0) )
		{
			re2:
			I2C_EEPROM_PAGE_WRITE(0X50,pwd_reg,str,8);
			for(i=0;i<100;i++)
			str[i]='\0';
			I2C_EEPROM_PAGE_READ(0X50,pwd_reg,str,8);
			UART0_STR(str);

			re3:
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			LCD_STR("ENTER PH. NO.:");	
			NUM_KEY(0XC0,10);
			UART0_STR("\r\n1=");
			UART0_STR(num_key);
			strncpy((char*)str,(char*)num_key,14);
			UART0_STR("\r\ncpy=");
			UART0_STR(str);
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			LCD_STR("RE-ENTER PH. NO.:");	
			NUM_KEY(0XC0,10);
			UART0_STR("\r\n2=");
			UART0_STR(num_key);
			if( (strcmp((const char*)str,(const char*)num_key) == 0) )
			{
				strcpy((char*)NUM,defaut_num);
				strncat((char*)NUM,(const char*)str,10);
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR("PH. NO. SAVED!!!");
				LCD_COMMAND(0XC0);
				LCD_STR(NUM);
				delay_milli(500);
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR(" DO YOU WANT TO");
				LCD_COMMAND(0XC0);
				LCD_STR("CHANGE PH.NO. OR");
				delay_milli(500);
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR("CONTINUE SETUP");
				LCD_COMMAND(0XC0);
				LCD_STR("(Y/N)->");
				ALPHA_KEY(0XC7,1);
				if( (strcmp("y",(const char*)str_key) == 0) )
				{
					re4:
					PH_NO_WRITE_EEPROM(ph_reg,(u8*)NUM);
					for(i=0;i<100;i++)
						str[i]='\0';
					I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
					UART0_STR(str);

					init_reotp:
					LCD_COMMAND(0X01);
					LCD_COMMAND(0X80);
					GSM_OTP(otp);
					sprintf((char*)str,"The OPT is %s",otp);
					GSM_SMS(str);
					LCD_STR("Enter OPT:");
					NUM_KEY(0xc0,4);
					if(0)
					{
						init_otp:
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
							LCD_STR("SAVED PH. NO.");
							LCD_COMMAND(0XC0);
							I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
							UART0_STR(str);
							LCD_STR((char*)str);
							delay_milli(500);
							LCD_COMMAND(0X01);
							LCD_COMMAND(0X80);
							LCD_STR("CHANGE PhNo(y/n)");
							LCD_COMMAND(0XBD);
							LCD_STR((char*)str);
							LCD_STR("->");
							ALPHA_KEY(0XCE,1);
							if( (strcmp("y",(const char*)str_key) == 0) )
								goto re3;
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
									goto init_reotp;
								}	 
							}
							
						}

					}
					delay_milli(500);
					if( (strcmp((const char*)otp,(const char*)num_key) == 0) )
					{
						otp_fail=0;
						delay_milli(500);
						LCD_COMMAND(0X01);
						LCD_COMMAND(0X80);
						LCD_STR("PH.NO. VERIFIED");
						LCD_COMMAND(0XC0);
						I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
						UART0_STR(str);
						LCD_STR((char*)str);

						delay_milli(500);
						LCD_COMMAND(0X01);
						LCD_COMMAND(0X80);
						LCD_STR("SETUP COMPLETED");
						LCD_COMMAND(0XC0);
						LCD_STR("   BOTH SAVED");
						
						delay_milli(500);
						LCD_COMMAND(0X01);
						LCD_COMMAND(0X80);
						LCD_STR("    ENJOY!!!");
						delay_milli(1000);
						
						LCD_COMMAND(0X01);	
					}	
					else
					{
						LCD_COMMAND(0X01);
						LCD_COMMAND(0X80);
						LCD_STR("INCORRECT OTP!!!");
						delay_milli(2500);
						goto init_otp;
					}	
				}
				else
				{
					delay_milli(500);
					LCD_COMMAND(0X01);
					LCD_COMMAND(0X80);
					LCD_STR("CHANGE PH. NO. ?");
					LCD_COMMAND(0XC0);
					LCD_STR("(y/n)->");
					ALPHA_KEY(0XC7,1);
					if( (strcmp("y",(const char*)str_key) == 0) )
						goto re3;
					else
						goto re4;
				}			 
			}
			else
			{
				LCD_COMMAND(0X01);
				LCD_COMMAND(0X80);
				LCD_STR("PHONE NO. DON'T");
				LCD_COMMAND(0XC0);
				LCD_STR("MATCH...RE-ENTER");
				delay_milli(1000);
				LCD_COMMAND(0X01);
				delay_milli(500);
				goto re3;
			}
		}
		else
		{
			delay_milli(500);
			LCD_COMMAND(0X01);
			LCD_COMMAND(0X80);
			LCD_STR("CHANGE PASSWORD?");
			LCD_COMMAND(0XC0);
			LCD_STR("(y/n)->");
			ALPHA_KEY(0XC7,1);
			if( (strcmp("y",(const char*)str_key) == 0) )
				goto re1;
			else
				goto re2;
		}
	}
	else
	{
		LCD_COMMAND(0X01);
		LCD_COMMAND(0X80);
		LCD_STR(" PASSWORD DON'T");
		LCD_COMMAND(0XC0);
		LCD_STR("MATCH...RE-ENTER");
		delay_milli(1000);
		LCD_COMMAND(0X01);
		delay_milli(500);
		goto re1;
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

	// I2C_EEPROM_PAGE_WRITE(0X50,pwd_reg,"classics",8);
	// PH_NO_WRITE_EEPROM(ph_reg,"+917987648081");

	UART0_STR("a");
	I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
	strncpy((char*)NUM,(const char*)str,14);
	I2C_EEPROM_PAGE_READ(0X50,pwd_reg,str,8);
	if( (DATA_ERROR((u8*)NUM)) || (DATA_ERROR((u8*)str)) )
	{
		INIT_SETUP();
	}
		
//	PH_NO_WRITE_EEPROM(ph_reg,"+917987648081");
	UART0_STR("b");
	
	
//	I2C_EEPROM_PAGE_READ(0X50,ph_reg,str,13);
	UART0_STR("c");
	UART0_STR(str);
//}
	
	for(i=0;i<100;i++)
		str[i]='\0';	
//	I2C_EEPROM_PAGE_WRITE(0X50,0X00,"classics",8);
//	delay_milli(100);
//	I2C_EEPROM_PAGE_READ(0X50,0X00,str,8);
//	UART0_STR(str);
//	if( (strcmp((const char*)str,"\0\0\0\0\0") == 0) )
//		 UART0_STR("\r\nyup\r\n");
//	if( (str[0] == 0xff) )
//		 UART0_STR("\r\nyup\r\n");
//	else
//	UART0_STR("\r\nnope\r\n");

	while(1)
	{
		for(i=0;i<100;i++)
		str[i]='\0';
		for(i=0;i<5;i++)
		otp[i]='\0';
//		LCD_COMMAND(0X01);
//		LCD_SCROLL("MULTI-LEVEL SECURITY SYSTEM",0x80,0x8f);
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

