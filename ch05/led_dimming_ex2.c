#include <stdio.h>
#include <pigpiod_if2.h> 
#include <unistd.h>
#define PINNO 13

#define DUTYCYCLE(x,range) x/(float)range*100

int main()
{
    int pi;
    int default_range = 50;
    int range;
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }
    set_PWM_range(pi, PINNO, default_range);
    range = get_PWM_range(pi, PINNO);

    printf("range:%d\n", range);

    for(int j = 0 ; j < 10 ; j++){
        for(int i = 0 ; i <= range ; i++) {
            set_PWM_dutycycle(pi, PINNO, i);
            time_sleep(0.02);
            printf("duty cycle:%.1f%% %d/%d\n", DUTYCYCLE(i,range), i, range);
        }
        for(int i = range ; i >= 0 ; i--) {
            set_PWM_dutycycle(pi, PINNO, i);
            time_sleep(0.02);
            printf("duty cycle:%.1f%% %d/%d\n", DUTYCYCLE(i,range), i, range);
        }
    }

    set_PWM_dutycycle(pi, PINNO, 0);

    pigpio_stop(pi);
    return 0;
}
