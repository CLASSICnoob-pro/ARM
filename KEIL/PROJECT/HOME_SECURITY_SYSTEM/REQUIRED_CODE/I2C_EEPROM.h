#ifndef __I2C_EEPROM_H__
#define __I2C_EEPROM_H__


#include<LPC21XX.H>
#include"types.h"
#include"I2C.h"
#include"delay.h"

void I2C_EEPROM_WRITE(u8 slaveAddr,u8 wBufAddr,u8 data);
u8 I2C_EEPROM_READ(u8 slaveAddr,u8 wBufAddr);
void I2C_EEPROM_PAGE_WRITE(u8 slaveAddr,u8 wBufAddr,u8* s,u8 size);
void I2C_EEPROM_PAGE_READ(u8 slaveAddr,u8 wBufAddr,u8* s,u8 size);

#endif
