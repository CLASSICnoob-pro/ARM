#ifndef __I2C_H__
#define __I2C_H__

#define I2CEN_BIT	6
#define START_BIT	5
#define STOP_BIT	4
#define SI_BIT		3
#define ACK_BIT		2

void I2C_INIT(void);
void I2C_START(void);
void I2C_RESTART(void);
void I2C_STOP(void);
void I2C_WRITE(unsigned char d);
unsigned char I2C_NACK(void);
unsigned char I2C_MASTERACK(void);

#endif
