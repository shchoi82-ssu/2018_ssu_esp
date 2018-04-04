#include <stdio.h>
#include <pigpiod_if2.h> 
#define PINNO 23
int count;
void cb_func(int pi, unsigned user_gpio, unsigned level, uint32_t tick)
{
    printf("count : %d\n", ++count);
    printf("pi:%d user_gpio:%d level:%d tick:%u\n\n", pi, user_gpio, level, tick);
}

int main()
{
    int pi;
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }

    set_mode(pi, PINNO, PI_INPUT);
    set_pull_up_down(pi, PINNO, PI_PUD_UP);

    // callback function 설정
    callback(pi, PINNO, FALLING_EDGE, cb_func);

    getchar();

    pigpio_stop(pi);
    return 0;
}
