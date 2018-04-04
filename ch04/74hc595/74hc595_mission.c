#include <stdio.h>
#include <pigpiod_if2.h>
#include "74hc595_functions.h"

int main(void)
{
    int ret;
    
    ret = init();
    if(ret == 0)
        return 0;

    uint8_t array[] = { 0b00011000,
                        0b00111100,
                        0b01111110,
                        0b11111111,
                        0b01111110,
                        0b00111100,
                        0b00011000,
                        0b00000000 };

    for(int i = 0 ; i < 100 ; i++){
        set8(array[i%8]);
        time_sleep(0.1);
    }

    release();
    return 0;
}
