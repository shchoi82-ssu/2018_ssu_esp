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
        allclear();
        set(i%8);
        time_sleep(0.1);
    }

    release();
    return 0;
}
