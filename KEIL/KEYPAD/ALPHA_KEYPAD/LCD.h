#ifndef __LCD_H__
#define __LCD_H__

#include "LPC21xx.H"
#include"delay.h"

#define LCD_D   0X0F<<20
#define RS      1<<17
#define RW      1<<18
#define E       1<<19

void LCD_COMMAND(char C);
void LCD_INIT(void);
void LCD_DATA(char D);
void LCD_INTEGER(int num);
void LCD_STR(char* s);

#endif
