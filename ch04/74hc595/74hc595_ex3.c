#include <stdio.h>
#include <pigpiod_if2.h>

#define SDATA       6    // Serial Input Data
#define STR_CLK     13   // Storage Register Clock(LATCH)
#define SHR_CLK     19   // Shift Register Clock
#define SHR_CLEAR   26   // Shift Register Clear

int main(void)
{
    int pi;
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }
    
    set_mode(pi, SDATA, PI_OUTPUT);
    set_mode(pi, STR_CLK, PI_OUTPUT);
    set_mode(pi, SHR_CLK, PI_OUTPUT);
    set_mode(pi, SHR_CLEAR, PI_OUTPUT);
    
    gpio_write(pi, SHR_CLEAR, 1);
    
    gpio_write(pi, SDATA, 1);	// serial data
    
    for(int i = 0 ; i < 8 ; i++){
        gpio_write(pi, SHR_CLK, 0);
        gpio_write(pi, SHR_CLK, 1);   // shift
        gpio_write(pi, STR_CLK, 0);
        gpio_write(pi, STR_CLK, 1);   // latch
        time_sleep(0.5);
    }

    pigpio_stop(pi);
    return 1;
}

