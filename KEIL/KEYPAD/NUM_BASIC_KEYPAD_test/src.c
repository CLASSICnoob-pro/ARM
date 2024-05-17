#include "LPC21xx.H"


#define COL 20
#define ROW 16

#define LCD_D   0X0F<<20
#define RS      1<<17
#define RW      1<<18
#define E       1<<19

#define delay 1000
#define main_sw ((IOPIN0 >> COL) & 0X07)

int c[] = {1<<(COL),1<<(COL+1),1<<(COL+2)};
int r[] = {1<<(ROW),1<<(ROW+1),1<<(ROW+2),1<<(ROW+3)};

int key[][4] = {{0,1,2},{3,4,5},{6,7,8},{9,10,11}};
char ch[][4] = {{'.'},              {'a','b','c'},      {'d','e','f'},
                {'g','h','i'},      {'j','k','l'},      {'m','n','o'},
                {'p','q','r','s'},  {'t','u','v'},      {'w','x','y','z'}};
char num[]   = {'1','2','3','4','5','6','7','8','9','*','0','#'};

int millis(void)
{
    T1PR = 15000-1;
    T1TCR = 0X01;
    return T1TC;
}

void delay_milli(int D)
{
    T0PR = 15000-1;
    T0TCR = 0X01;
    while(T0TC < D);
    T0TCR = 0X03;
    T0TCR = 0X00;
}

void LCD_COMMAND(char C)
{
    IOCLR1 = LCD_D;
    IOCLR1 = RS;
    IOCLR1 = RW;
    IOSET1 = (C&0XF0)<<16;
    IOSET1 = E;
    delay_milli(10);
    IOCLR1 = E;
    
    IOCLR1 = LCD_D;
    IOSET1 = (C&0X0F)<<20;
    IOSET1 = E;
    delay_milli(10);
    IOCLR1 = E;
}

void LCD_INIT()
{
    IODIR1 |= LCD_D|RS|RW|E;
    LCD_COMMAND(0X01);
    LCD_COMMAND(0X02);
    LCD_COMMAND(0X0C);
    LCD_COMMAND(0X28);
    LCD_COMMAND(0X80);
}

void LCD_DATA(char D)
{
    IOCLR1 = LCD_D;
    IOSET1 = RS;
    IOCLR1 = RW;
    IOSET1 = (D&0XF0)<<16;
    IOSET1 = E;
    delay_milli(10);
    IOCLR1 = E;
    
    IOCLR1 = LCD_D;
    IOSET1 = (D&0X0F)<<20;
    IOSET1 = E;
    delay_milli(10);
    IOCLR1 = E;
}

void LCD_INTEGER(int num)
{
    int i=0;
    char c[10];
    if(num==0)
        LCD_DATA('0');
    else
    {
        if(num<0)
        {
            LCD_DATA('-');
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
		    LCD_DATA(c[i]);
	    }
    }
}

int keypad(void)
{
    int i,j;
    for(i=0;i<4;i++)
    {
        IOSET0 = 0X0F<<ROW;
        IOCLR0 = r[i];
        for(j=0;j<3;j++)
        {
            if(!(IOPIN0 & (c[j])))
			{
				IOCLR0 = 0X0F<<ROW;
                return key[i][j];
			}
        }
    }
		return -1;
}

void LCD_STR(char* s)
{
    while(*s)
        LCD_DATA(*s++);
}

void UART0_INIT(void)
{
    PINSEL0 |= 0X05;
    U0LCR = 0X83;
    U0DLL = 97;
    U0LCR = 0X03;
}

void UART0_TX(char c)
{
    while((U0LSR>>5 & 1) == 0);
    U0THR = c;
}

void UART0_INT(int num)
{
    int i=0;
    char c[10];
	if(num==0)
	    UART0_TX('0');
	else
	{
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

int main()
{
	int sw_c, sw_p = 1, count = -1;
    int press = 0;
	
	UART0_INIT();
	LCD_INIT();
	
	IODIR0 |= r[0]|r[1]|r[2]|r[3];

	while(1){
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR("ENTER NUMBER:");
	LCD_COMMAND(0XC0);
    LCD_INTEGER(count);
	
    if((sw_c= (main_sw == 0X07))!= sw_p)
    {
    	if(sw_c==0 && sw_p==1)
    	{
//    	    if((millis()-press)<delay)
//    	        count++;
//    	    else
//    	        count=0;
            count=keypad();
    	}
    	else
    	    count = -1;
//    	    press = millis();
    	sw_p = sw_c;
    }
    UART0_INT(count);
    UART0_TX('\n');
}
}
