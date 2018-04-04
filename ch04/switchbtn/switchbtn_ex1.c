#include <stdio.h>
#include <pigpiod_if2.h> 
#define PINNO 23

int main()
{
    int pi;
    int level;
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }

    set_mode(pi, PINNO, PI_INPUT);
    set_pull_up_down(pi, PINNO, PI_PUD_DOWN);

    while(1){
        level = gpio_read(pi, PINNO);
        printf("%s %d\n", level?"--__--":"------", level);
        time_sleep(0.1);
    }

    pigpio_stop(pi);
    return 0;
}
