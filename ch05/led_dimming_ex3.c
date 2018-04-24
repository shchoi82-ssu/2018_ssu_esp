#include <stdio.h>
#include <pigpiod_if2.h> 
#include <unistd.h>

#define DUTYCYCLE(x,range) x/(float)range*100

int pi;
int range;
int gpios[] = {26,19};
int default_range = 100;
void* dimming_thread(void* arg)
{
    int gpio = (int)arg;
    printf("dimming_thread start gpio:%d\n", gpio);
    for(int i = 0 ; i <= range ; i++){
        set_PWM_dutycycle(pi, gpio, i);
        time_sleep(0.007);
    }

    for(int i = range ; i >= 0 ; i--){
        set_PWM_dutycycle(pi, gpio, i);
        time_sleep(0.007);
    }
    printf("dimming_thread exit gpio:%d\n", gpio);
    return NULL;
}

int main()
{
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 1;
    }
    for(int i = 0 ; i < 2 ; i++){
        set_PWM_range(pi, gpios[i], default_range);
        range = get_PWM_range(pi, gpios[i]);
    }
    printf("range:%d\n", range);

    for(int i = 0 ; ; i++){
        start_thread(dimming_thread, (void*)gpios[i%2]);
        time_sleep(1);
    }

    pigpio_stop(pi);
    return 0;
}
