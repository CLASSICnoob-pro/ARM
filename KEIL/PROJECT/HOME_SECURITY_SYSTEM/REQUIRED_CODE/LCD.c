#include"LCD.h"

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

void LCD_STR(char* s)
{
    while(*s)
        LCD_DATA(*s++);
}

int LCD_SCROLL(char* s,int start,int stop)
{
	int i=0,pos,j;
	for(i=0;s[i];i++)
	{
		for(j=i,pos=start;(pos<stop)&&s[j];pos++,j++)
		{
			LCD_COMMAND(pos);
			LCD_DATA(s[j]);
		}
		if(pos!=stop)
		{
			LCD_COMMAND(pos);
			LCD_DATA('\0');
		}
		//delay_milli(50);
	}
	return 0;
}
