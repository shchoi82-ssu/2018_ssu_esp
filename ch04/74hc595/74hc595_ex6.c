#include <stdio.h>
#include <pigpiod_if2.h>
#include "74hc595_functions.h"

int main(void)
{
    int ret;
    
    ret = init();
    if(ret == 0)
        return 0;

    for(int i = 0 ; i < 100 ; i++){
        set8(0b10101010);
        time_sleep(0.1);
        set8(0b01010101);
        time_sleep(0.1);
    }

    release();
    return 0;
}
