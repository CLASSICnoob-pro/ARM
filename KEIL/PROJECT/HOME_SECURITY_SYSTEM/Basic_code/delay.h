#ifndef __DELAY_H__
#define __DELAY_H__

#include<lpc21xx.h>

int millis(void);
void delay_micro(int D);
void delay_milli(int D);
void delay_sec(int D);

#endif
