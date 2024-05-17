#include<lpc21xx.h>

#define IN1 1<<4
#define IN2 1<<5
#define EN1 1<<6

void L293D_INIT(void);
void DOOR_OPEN(void);
void DOOR_CLOSE(void);
void DOOR_IDLE(void);
