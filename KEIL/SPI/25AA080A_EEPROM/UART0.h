#ifndef __UART0_H__
#define __UART0_H__

#include<lpc21xx.h>
#include<types.h>


void UART0_INIT(void);
void UART0_TX(u8 D);
u8 UART0_RX(void);
void UART0_STR(u8* S);

#endif
