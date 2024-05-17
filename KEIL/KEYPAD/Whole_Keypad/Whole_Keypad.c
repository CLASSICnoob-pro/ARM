#include "Whole_Keypad.h"

int c[] = {1<<(COL),1<<(COL+1),1<<(COL+2)};
int r[] = {1<<(ROW),1<<(ROW+1),1<<(ROW+2),1<<(ROW+3)};

u8 n[20];
u8 str[100];

int key[][4] = {{0,1,2},{3,4,5},{6,7,8},{9,10,11}};
char ch[][4] = {{'.'},              {'a','b','c'},      {'d','e','f'},
                {'g','h','i'},      {'j','k','l'},      {'m','n','o'},
                {'p','q','r','s'},  {'t','u','v'},      {'w','x','y','z'},
                {'*'},  			{' '},    			{'#'}			};
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

void ALPHA(int init_pos)
{
	int sw_c, sw_p = 1, count = 0,flag = -1;
	int p_key = -1 ,c_key,pos = init_pos-1;
    int press = 0,hide = 0,n_pos=0;
	char c;
	
	while( (ch[p_key][count]) != '#')
	{
	    LCD_COMMAND(pos);

		c=ch[p_key][count];
		if((flag==1) && (c!='*'))
		{
			LCD_COMMAND(pos);
			LCD_DATA(c);
			str[n_pos-1]=c;
			hide = millis();
			flag=0;
		}
		else if( ((millis()-hide)>delay)  && (c!='*') && (flag==0)) 
			LCD_DATA('*');
				
		if(c=='*')
			p_key = -1;		
	
		if((sw_c= (main_sw == 0X07))!= sw_p)
	    {
	    	if(sw_c==0 && sw_p==1)
	    	{
	    	    if((p_key == (c_key=keypad()) && (millis()-press)<delay) )
	    	    {
				    count++;
					if((c=ch[p_key][count]) == '\0')
						count=0;
					flag=1;
				}
	    	    else
				{
					LCD_COMMAND(pos);
					if (flag==0)
						LCD_DATA('*');
					flag=1;
	    	        count=0;
					if(ch[c_key][count]!= '*')
					{
						pos++;
						n_pos++;
					}
					else
					{
						LCD_COMMAND(pos);
						LCD_DATA('\0');
						str[n_pos]='\0';
						if(pos!=init_pos-1)
						{ 	
							pos--;
							n_pos--;
						}
						flag=-1;				
					}
				}
	           	p_key = c_key;
	    	}
	    	else
				press = millis();
			
	    	sw_p = sw_c;
	    }
	}
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
    }
}

