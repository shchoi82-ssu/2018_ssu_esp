#include <pigpiod_if2.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int pi;
    int gpio;
    float delay;

    if(argc != 3){
        printf("usage : a.out gpio# delay(s)\n");
        return 1;
    }

    gpio = atoi(argv[1]);
    delay = atof(argv[2]);

    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "pigpio_start error\n");
        return 1;
    }

    set_mode(pi, gpio, PI_OUTPUT);

    set_servo_pulsewidth(pi, gpio, 500);
    time_sleep(1);
    int dir = 1;
    int i = 500;
    while(1){
        set_servo_pulsewidth(pi, gpio, i);
        printf("pulse_width:%d degree:%d\n", i, (int)((i-500)/11.11111));
        time_sleep(delay);
        if(dir == 1)    i++;
        else            i--;
        if(499 == i){    
            i=500; 
            dir=1;  
        } else if(2501 == i){ 
            i=2500; 
            dir=0;  
        }
    }

    pigpio_stop(pi);

    return 0;
}
