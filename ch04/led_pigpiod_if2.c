#include <stdio.h>
#include <pigpiod_if2.h> 
#include <stdlib.h>
#define GPIO 13

int main()
{
    int pi;
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        exit(-1);
    }

    set_mode(pi, GPIO, PI_OUTPUT);
    printf("gpio:%d\n", GPIO);

    for(int i = 0 ; i < 5 ; i++){
        gpio_write(pi, GPIO, 1);
        printf("LED ON\n");
        time_sleep(1);
        gpio_write(pi, GPIO, 0);
        printf("LED OFF\n");
        time_sleep(1);
    }

    pigpio_stop(pi);
    return 0;
}
