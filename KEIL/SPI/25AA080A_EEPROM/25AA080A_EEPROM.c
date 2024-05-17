#include<lpc21xx.h>
#include<types.h>
#include"delay.h"
#include"UART0.h"

#define SEL_BIT 	1<<7
#define SPIF_BIT	((S0SPSR>>7)&1)
#define WREN		0X06
#define WRDI		0X04
#define READ		0X03
#define WRITE		0X02

void SPI_INIT()
{
	PINSEL0 |= 0X1500;
	IODIR0  |= SEL_BIT;
	S0SPCCR  = 150;
	S0SPCR	 = 0X20;
	IOSET0 	 = SEL_BIT;
}

void SPI_EEPROM_WREN()
{
	IOCLR0 = SEL_BIT;
	S0SPDR = WREN;
	while(SPIF_BIT == 0);
	IOSET0 = SEL_BIT;
}

void SPI_EEPROM_WRDI()
{
	IOCLR0 = SEL_BIT;
	S0SPDR = WRDI;
	while(SPIF_BIT == 0);
	IOSET0 = SEL_BIT;
}

void SPI_EEPROM_TRANSFER(u8 D)
{
	u8 stat;
    stat = S0SPSR;
	S0SPDR = D;
	while(SPIF_BIT == 0);
}

u8 SPI_EEPROM_RECEIVE()
{
	
	//while(SPIF_BIT == 0);
	//delay_milli(100);
	return S0SPDR;
}

//u8 SPI0(u8 data)
//{
//   u8 stat;
//   stat = S0SPSR;    //clear SPIF 
//   S0SPDR = data;   // load spi tx reg
//   while(READBIT(S0SPSR,SPIF_BIT)==0); // wait for transmission to complete
//   return S0SPDR;    // read data from SPI data reg, place into buffer 
//}

void SPI_EEPROM_WRITE(u8 D,u8 buffAddr)
{
	SPI_EEPROM_WREN();
	IOCLR0 = SEL_BIT;
	SPI_EEPROM_TRANSFER(WRITE);
	SPI_EEPROM_TRANSFER(0X00);
	SPI_EEPROM_TRANSFER(buffAddr);
	IOSET0 = SEL_BIT;
	SPI_EEPROM_WRDI();
}

u8 SPI_EEPROM_READ(u8 buffAddr)
{
	u8 D;
	IOCLR0 = SEL_BIT;
	SPI_EEPROM_TRANSFER(READ);
	SPI_EEPROM_TRANSFER(0X00);
	SPI_EEPROM_TRANSFER(buffAddr);
	D = SPI_EEPROM_RECEIVE();
	//while(SPIF_BIT == 0);
	IOSET0 = SEL_BIT;
	return D;
}

int main()
{
	u8 d;
	SPI_INIT();
	UART0_INIT();
	UART0_STR("Hi\r\n");
	SPI_EEPROM_WRITE('H',0x00);
	delay_milli(100);
	UART0_STR("Hi\r\n");
	d= SPI_EEPROM_READ(0x00);
	UART0_STR("Hi\r\n");
	UART0_TX(d);
}
