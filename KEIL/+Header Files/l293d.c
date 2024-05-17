#include"l293d.h"

void L293D_INIT(void)
{
	IODIR0 |= IN1|IN2|EN1;
}

void DOOR_OPEN(void)
{
	IOSET0 = IN1;
	IOCLR0 = IN2;
	IOSET0 = EN1;
}

void DOOR_CLOSE(void)
{
	IOCLR0 = IN1;
	IOSET0 = IN2;
	IOSET0 = EN1;
}

void DOOR_IDLE(void)
{
	IOCLR0 = IN1;
	IOCLR0 = IN2;
	IOCLR0 = EN1;
}
