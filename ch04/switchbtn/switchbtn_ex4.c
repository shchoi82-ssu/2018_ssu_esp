#include <stdio.h>
#include <pigpiod_if2.h> 
#define PINNO 23
int count;
void cb_falling_func(int pi, unsigned user_gpio, unsigned level, uint32_t tick)
{
//    printf("cb_falling_func gpio:%d count:%d\n", user_gpio, count);
}

void cb_rising_func(int pi, unsigned user_gpio, unsigned level, uint32_t tick)
{
//    printf("cb_rising_func gpio:%d count:%d\n", user_gpio, count);
    count++;
}

int main()
{
    int pi;
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));  return 1;
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
