#include "LPC21xx.H"
#include "LCD.h"
#include "UART0.h"
#include "delay.h"

#define COL 14
#define ROW 17

#define LCD_D   0X0F<<20
#define RS      1<<17
#define RW      1<<18
#define E       1<<19

#define delay 1000
#define main_sw ((IOPIN0 >> COL) & 0X07)

void KEYPAD_INIT(void);
int keypad(void);
void ALPHA_KEY(int,int);
void NUM_KEY(int,int);
