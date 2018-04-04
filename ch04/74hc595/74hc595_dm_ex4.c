#include <stdio.h>
#include <pigpio.h>
#include "74hc595_functions.h"

int main(void)
{
    uint16_t tmp;
    uint8_t rows[] = {  0b01111111,
                        0b10111111,
                        0b11011111,
                        0b11101111,
                        0b11110111,
                        0b11111011,
                        0b11111101,
                        0b11111110};

    uint8_t cols[] = {  0b00011000,
                        0b00011000,
                        0b00011000,
                        0b11111111,
                        0b11111111,
                        0b00011000,
                        0b00011000,
                        0b00011000};

    int i, ret;
    ret = init();
    if(ret == 0)
        return 0;

    for(;;)
        for(i = 0 ; i < 8 ; i++){
			tmp = (rows[i]<<8) | cols[i];
            set16(tmp);
            gpioDelay(1000);
        }
 
    release();
    return 0;
}
