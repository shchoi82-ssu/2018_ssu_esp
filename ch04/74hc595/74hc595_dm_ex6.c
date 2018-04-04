#include <stdio.h>
#include <pigpio.h>
#include "74hc595_functions.h"

void dot(int row, int col)
{
    uint8_t row8, col8;
    uint16_t tmp;
    row8 = ~(1 << (8-row));
    col8 = 1 << (8-col);
    tmp = (row8<<8) | col8;
    set16(tmp);
}

int cnt;
void callback(void)
{
	printf("500 milliseconds have elapsed\n");
	cnt++;	
}
 
int main(void)
{
    int i,j;

    char mat[8][8] ={ 0,0,0,0,0,0,0,0,
                      0,1,1,0,0,1,1,0,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      0,1,1,1,1,1,1,0,
                      0,0,1,1,1,1,0,0,
                      0,0,0,1,1,0,0,0};


    int ret;
    ret = init();
    if(ret == 0)
        return 0;

	gpioSetTimerFunc(0, 500, callback);

    while(1)
        for(i = 0 ; i < 8 ; i++)
            for(j = 0 ; j < 8 ; j++)
                if(mat[i][j] == cnt%2)
                    dot(i+1,j+1);


    release();
    return 0;
}

