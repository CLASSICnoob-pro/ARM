#include<stdio.h>
#include"LPC21XX.H"

#define press IOPIN0>>

char key[][4]={     {'1'},                   {'a','b','c'},     {'d','e','f'},
                    {'g','h','i'},           {'j','k','l'},     {'m','n','o'},
                    {'p','q','r','s'},       {'t','u','v'},     {'w','x','y','z'},
                    {'*'},                   {'0'},             {'#'}               };

void delay(int d)
{
    T0PR = 15000-1;
    T0TCR = 0X01;
    while(T0TC < d);
    T0TCR = 0X03;
    T0TCR = 0X00;
}

int main()
{
    // for(int i=0;i<9;i++)
    // {
    //     for(int j=0;j<4;j++)
    //     {
    //         printf("%c ",key[i][j]);
    //     }
    //     printf("\n");
    // }

    // IODIR0 = 4;
    while (1){
        IOPIN0 ^= (4);
        delay(500); 
    }

}