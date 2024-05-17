#include "Whole_Keypad.h"

int c[] = {1<<(COL),1<<(COL+1),1<<(COL+2)};
int r[] = {1<<(ROW),1<<(ROW+1),1<<(ROW+2),1<<(ROW+3)};

u8 num_key[20];
u8 str_key[100];

int key[][4] = {{0,1,2},{3,4,5},{6,7,8},{9,10,11}};
char ch[][4] = {{'.'},              {'a','b','c'},      {'d','e','f'},
                {'g','h','i'},      {'j','k','l'},      {'m','n','o'},
                {'p','q','r','s'},  {'t','u','v'},      {'w','x','y','z'},
                {'*'},  			{' '},    			{'#'}			};
char num[]   = {'1','2','3','4','5','6','7','8','9','*','0','#'};

void KEYPAD_INIT()
{
	IODIR0 |= r[0]|r[1]|r[2]|r[3];
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

void ALPHA_KEY(int init_pos,int limit)
{
	int sw_c, sw_p = 1, count = 0,flag = -1;
	int last_key=0,p_key = -1 ,c_key,pos = init_pos-1;
    int press = 0,hide = 0,n_pos=0;
	char c;
	for(n_pos=0;n_pos<100;n_pos++)
		str_key[n_pos]='\0';
	n_pos=0;
	while(1)
	{	if( (ch[p_key][count]) == '#')
			if(n_pos==limit)
				return;
			else
			{
				if(limit==8)
				{
					LCD_COMMAND(0XC8);
					LCD_STR("(8-PLZ)");
				}
				else if(limit==1)
				{
					LCD_COMMAND(0XC8);
					LCD_STR("(yes/no)");
				}
				else{
					LCD_COMMAND(0XCB);
					LCD_DATA('(');
					LCD_INTEGER(limit);
					LCD_DATA(')');
				}
			}

	    LCD_COMMAND(pos);

		c=ch[p_key][count];
		if((flag==1) && (c!='*'))
		{
			LCD_COMMAND(pos);
			LCD_DATA(c);
			str_key[n_pos-1]=c;
			last_key=p_key;
			hide = millis();
			flag=0;
		}
		else if( ((millis()-hide)>delay)  && (c!='*') && (flag==0)) 
		{
			LCD_DATA('*');
			last_key=-1;
		}
				
		if(c=='*')
			p_key = -1;		
	
		if((sw_c= (main_sw == 0X07))!= sw_p)
	    {
	    	if(sw_c==0 && sw_p==1)
	    	{
	    	    if((p_key == (c_key=keypad()) && (millis()-press)<delay) )
	    	    {
				    count++;
							flag=1;
					if( ((c=ch[p_key][count]) == '\0') || (count==4))
						count=0;
					if( (n_pos==limit) || (ch[c_key][count]== '#') )
							if( (last_key!=p_key) && (last_key==-1) )
								flag=0;
					if(ch[c_key][count]== '#')
						flag=-1;
				}
	    	    else
				{
					if (flag==0)
					{
						LCD_COMMAND(pos);
						LCD_DATA('*');
					}
					flag=1;
	    	        count=0;
					if(ch[c_key][count]!= '#')
					{
						if(ch[c_key][count]!= '*')
						{
							if(n_pos!=limit)
							{
								pos++;
								n_pos++;
							}
							else
							flag=0;
						}
						else
						{
							LCD_COMMAND(pos);
							LCD_DATA('\0');
							str_key[n_pos-1]='\0';
							if(pos!=init_pos-1)
							{ 	
								pos--;
								n_pos--;
							}
							flag=-1;				
						}
					}
					else
					flag=-1;
				}
	           	p_key = c_key;
	    	}
	    	else
				press = millis();
			
	    	sw_p = sw_c;
	    }
	}
}

void NUM_KEY(int init_pos,int limit)
{
	int sw_c, sw_p = 1,flag=-1;
	int key=-1,pos = init_pos-1;
    int hide=0,n_pos=0;
	char c;
	delay_milli(200);
	for(n_pos=0;n_pos<20;n_pos++)
		num_key[n_pos]='\0';
	n_pos=0;
	while(1)
	{	if( (num[key]) == '#')
			if(n_pos==limit)
				return;
			else
			{
				if(limit==4)
				{
					LCD_COMMAND(0XC6);
					LCD_STR("(4-digits)");
				}
				else if(limit==10)
				{
					LCD_COMMAND(0XCC);
					LCD_STR("(10)");
				}
				else if(limit!=1){
					LCD_COMMAND(0XCB);
					LCD_DATA('(');
					LCD_INTEGER(limit);
					LCD_DATA(')');
				}
			}
	    LCD_COMMAND(pos);
	
		c=num[key];
		if((flag==1) && (c!='*'))
		{
			LCD_COMMAND(pos);
			LCD_DATA(c);
			num_key[n_pos-1] = c;
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
				if( (n_pos!=limit) && (num[key]!= '#') )
					flag=1;
				if(num[key]!= '#')
				{
					if(num[key]!= '*')
					{
						if(n_pos!=limit)
						{
							pos++;
							n_pos++;
						}
						else
							flag=0;
					}
					else
					{
						LCD_COMMAND(pos);
						LCD_DATA('\0');
						num_key[n_pos-1] = '\0';
						if(pos!=init_pos-1)
						{
							pos--;
							n_pos--;
						}
						flag=-1;				
					}
				}
				else
					flag=-1;
			}
	        sw_p = sw_c;   	
	    }
    }
}

