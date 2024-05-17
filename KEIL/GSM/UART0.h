#ifndef __UART0_H__
#define __UART0_H__

#include"types.h"

void UART0_INIT(void);
void UART0_TX(u8);
u8 UART0_RX(void);
void UART0_STR(u8*);

#endif
