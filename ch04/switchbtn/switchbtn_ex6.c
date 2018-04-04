#include <stdio.h>
#include <pigpiod_if2.h> 
#define PINNO 23
int count;
uint32_t falling_tick, rising_tick;
void cb_falling_func(int pi, unsigned user_gpio, unsigned level, uint32_t tick)
{
    falling_tick = tick;
}

void cb_rising_func(int pi, unsigned user_gpio, unsigned level, uint32_t tick)
{
    rising_tick = tick;
    if(rising_tick-falling_tick > 10000)
        printf("count:%d period:%d us\n", ++count, rising_tick-falling_tick);
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
    callback(pi, PINNO, FALLING_EDGE, cb_falling_func);
    callback(pi, PINNO, RISING_EDGE, cb_rising_func);

    getchar();
    printf("버튼 누른 총 횟수:%d\n", count);

    pigpio_stop(pi);
    return 0;
}
