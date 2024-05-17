#include<LPC21XX.H>
#include"I2C.h"

void I2C_INIT()
{
	PINSEL0 |= 0X50;
	I2SCLL = (15000000/100000)/2;
	I2SCLH = (15000000/100000)/2;
	I2CONSET = 1<<I2CEN_BIT;
}

void I2C_START()   
{
	I2CONSET = 1<<START_BIT;
	while(((I2CONSET>>SI_BIT) & 1) == 0);
	I2CONCLR = 1<<START_BIT;	
}

void I2C_RESTART()
{
	I2CONSET = 1<<START_BIT;
	I2CONCLR = 1<<SI_BIT;
	while(((I2CONSET>>SI_BIT) & 1) == 0);
	I2CONCLR = 1<<START_BIT;	
}						

void I2C_STOP()
{
	I2CONSET = 1<<STOP_BIT;
	I2CONCLR = 1<<SI_BIT;	
}	

void I2C_WRITE(unsigned char d)
{
	I2DAT = d;
	I2CONCLR = 1<<SI_BIT;
	while(((I2CONSET>>SI_BIT) & 1) == 0);

}	

unsigned char I2C_NACK(void)
{
	I2CONCLR = 1<<ACK_BIT;
	I2CONCLR = 1<<SI_BIT;
	while(((I2CONSET>>SI_BIT) & 1) == 0);
	return I2DAT;
}

unsigned char I2C_MASTERACK(void)
{
	I2CONSET = 1<<ACK_BIT;
	I2CONCLR = 1<<SI_BIT;
	while(((I2CONSET>>SI_BIT) & 1) == 0);
	I2CONCLR = 1<<ACK_BIT;
	return I2DAT;
}	
