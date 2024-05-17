#include"i2c_eeprom.H"

void I2C_EEPROM_WRITE(u8 slaveAddr,u8 wBufAddr,u8 data)
{
	I2C_START();
	I2C_WRITE((slaveAddr<<1) | 0);
	I2C_WRITE(wBufAddr);
	I2C_WRITE(data);
	I2C_STOP();
}

u8 I2C_EEPROM_READ(u8 slaveAddr,u8 wBufAddr)
{
	u8 c;
	I2C_START();
	I2C_WRITE((slaveAddr<<1) | 0);
	I2C_WRITE(wBufAddr);
	I2C_RESTART();
	I2C_WRITE((slaveAddr<<1) | 1);
	c=I2C_NACK();
	I2C_STOP();
	return c;
}

void I2C_EEPROM_PAGE_WRITE(u8 slaveAddr,u8 wBufAddr,u8* s,u8 size)
{
	u8 i;
	I2C_START();
	I2C_WRITE((slaveAddr<<1) | 0);
	I2C_WRITE(wBufAddr);

	for(i=0;i<size;i++)
		I2C_WRITE(s[i]);

	I2C_STOP();
}

void I2C_EEPROM_PAGE_READ(u8 slaveAddr,u8 wBufAddr,u8* s,u8 size)
{
	u8 i;
	I2C_START();
	I2C_WRITE((slaveAddr<<1) | 0);
	I2C_WRITE(wBufAddr);
	I2C_RESTART();
	I2C_WRITE((slaveAddr<<1) | 1);
	for(i=0;i<size-1;i++)
		s[i]=I2C_MASTERACK();
	s[i]=I2C_NACK();
	I2C_STOP();
}

