#include "LPC21xx.H"
#include "LCD.h"
#include "UART0.h"
#include "delay.h"

#define COL 20
#define ROW 16

#define LCD_D   0X0F<<20
#define RS      1<<17
#define RW      1<<18
#define E       1<<19

#define delay 1000
#define main_sw ((IOPIN0 >> COL) & 0X07)

int keypad(void);
void ALPHA(int init_pos);
void NUM(int init_pos);
