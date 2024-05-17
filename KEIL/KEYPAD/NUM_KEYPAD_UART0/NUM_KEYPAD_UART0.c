#include "LPC21xx.H"
#include "lcd.h"
#include "uart0.h"
#include "delay.h"

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
u8 n[20];

int key[][4] = {{0,1,2},{3,4,5},{6,7,8},{9,10,11}};
/*char ch[][4] = {{'.'},              {'a','b','c'},      {'d','e','f'},
                {'g','h','i'},      {'j','k','l'},      {'m','n','o'},
                {'p','q','r','s'},  {'t','u','v'},      {'w','x','y','z'},
                {'*'},  			{' '},    			{'#'}			}; */
char num[]   = {'1','2','3','4','5','6','7','8','9','*','0','#'};


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

void NUM(int init_pos)
{
	int sw_c, sw_p = 1,flag=-1;
	int key=-1,pos = init_pos-1;
    int hide=0,n_pos=0;
	char c;
	while( (num[key]) != '#'){
	    LCD_COMMAND(pos);
	
		c=num[key];
		if((flag==1) && (c!='*'))
		{
			LCD_COMMAND(pos);
			LCD_DATA(c);
			n[n_pos-1] = c;
			hide = millis();
			flag=0;
		}
		else if( ((millis()-hide)>delay)  && (c!='*') && (flag==0)) 
		{
			LCD_DATA('*');
			
		}
		
		if(c=='*')
		{
			key = -1;		
		}		  
	
		if((sw_c= (main_sw == 0X07))!= sw_p)
	    {
	    	if(sw_c==0 && sw_p==1)
	    	{
	    	    key=keypad();
				
				if (flag==0)
				{
					LCD_COMMAND(pos);
					LCD_DATA('*');
				}
				flag=1;
	    	    if(num[key]!= '*')
				{
					pos++;
					n_pos++;
				}
				else
				{
					LCD_COMMAND(pos);
					LCD_DATA('\0');
					n[n_pos-1] = '\0';
					if(pos!=init_pos-1)
					{
					 	pos--;
						n_pos--;
					}
					flag=-1;				
				}
			}
	        sw_p = sw_c;   	
	    }
	//    else
	//    	    count = -1;
	  //  	    press = millis();
			
	    
    }
	


}

int main()
{	
	IODIR0 |= r[0]|r[1]|r[2]|r[3];
	UART0_INIT();
	LCD_INIT();
	
		//UART0_TX('A');
	//UART0_STR("ENTER NUMBER:");

	LCD_COMMAND(0X80);
	LCD_STR("ENTER NUMBER:");
	UART0_STR("ENTER NUMBER:\r\n");		       
	NUM(0xc0);
	UART0_STR("ENTER NUMBER:");
	UART0_STR(n);
}

