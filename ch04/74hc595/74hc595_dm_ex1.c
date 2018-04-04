#include <stdio.h>
#include <pigpiod_if2.h>
#include "74hc595_functions.h"

int main(void)
{

    int i, ret;
    uint16_t tmp;
    uint8_t row[] = {   0b01111111,
                        0b10111111,
                        0b11011111,
                        0b11101111,
                        0b11110111,
                        0b11111011,
                        0b11111101,
                        0b11111110};
    uint8_t col = 0b11111111;
    ret = init();
    if(ret == 0)
        return 0;

    for(;;)
        for(i = 0 ; i < 8 ; i++){
            tmp = (row[i]<<8) | col;
            set16(tmp);
            time_sleep(0.1);
        }

    release();
    return 0;
}
