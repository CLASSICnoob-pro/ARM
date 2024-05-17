#include"UART0.H"

void UART0_INIT(void)
{
	PINSEL0|= 0X05;
	U0LCR = 0X83;
	U0DLL = 97;
	U0LCR = 0X03;
}

void UART0_TX(u8 D)
{
	while(((U0LSR>>5)&1) == 0);
	U0THR = D;
}

u8 UART0_RX(void)
{
	while((U0LSR&1) == 0);
	return U0RBR;
}

void UART0_STR(u8* S)
{
	while(*S)
		UART0_TX(*S++);
}

void UART0_INTEGER(int num)
{
    int i=0;
    char c[10];
    if(num==0)
        UART0_TX('0');
    else
    {
        if(num<0)
        {
            UART0_TX('-');
            num=-num;
        }
        
        while(num>0)
		{
			c[i]= (num%10)+48;
			num/=10;
			i++;
	    }
	    while(--i>=0)
	    {
		    UART0_TX(c[i]);
	    }
    }
}

