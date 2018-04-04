#include <stdio.h>
#include <pigpiod_if2.h>
#include "74hc595_functions.h"

int main(void)
{
    int i;
    uint8_t arr[] = {   0b10000000,
                        0b01000000,
                        0b00100000,
                        0b00010000,
                        0b00001000,
                        0b00000100,
                        0b00000010,
                        0b00000001};

    int ret;
    
    ret = init();
    if(ret == 0)
        return 0;

    for(int i = 0 ; i < sizeof(arr) ; i++){
        set8(arr[i]);
        time_sleep(0.5);
    }

    release();
    return 0;
}
