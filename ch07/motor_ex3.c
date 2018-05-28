#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

#define INPUT1 5
#define INPUT2 6
#define INPUT3 17
#define INPUT4 27

int main(void)
{
    int i;
	int pi;
	if((pi = pigpio_start(NULL, NULL)) < 0){
		fprintf(stderr, "%s\n", pigpio_error(pi));
		exit(-1);
	}

    set_mode(pi, INPUT1, PI_OUTPUT);
    set_mode(pi, INPUT2, PI_OUTPUT);
    set_mode(pi, INPUT3, PI_OUTPUT);
    set_mode(pi, INPUT4, PI_OUTPUT);

    for(i = 0 ; i < 5 ; i++){
        gpio_write(pi, INPUT1, PI_HIGH);    // A 방향
        gpio_write(pi, INPUT2, PI_LOW);
        gpio_write(pi, INPUT3, PI_HIGH);    // A 방향
        gpio_write(pi, INPUT4, PI_LOW);
        time_sleep(1);
        gpio_write(pi, INPUT1, PI_LOW);     // B 방향
        gpio_write(pi, INPUT2, PI_HIGH);
        gpio_write(pi, INPUT3, PI_LOW);     // B 방향
        gpio_write(pi, INPUT4, PI_HIGH);
        time_sleep(1);
    }

    gpio_write(pi, INPUT1, PI_LOW);
    gpio_write(pi, INPUT2, PI_LOW);
    gpio_write(pi, INPUT3, PI_LOW);
    gpio_write(pi, INPUT4, PI_LOW);

    return 0;
}

