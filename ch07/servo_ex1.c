#include <pigpiod_if2.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

	int pi;
    int gpio;
    int degree;
    int pulse_width;

    if(argc != 3){
        printf("usage : a.out gpio# degree\n");
        return 1;
    }

    gpio = atoi(argv[1]);
    degree = atoi(argv[2]);

	if((pi = pigpio_start(NULL, NULL)) < 0){
		fprintf(stderr, "pigpio_start error\n");
		return 1;
	}

    set_mode(pi, gpio, PI_OUTPUT);

    if(degree >= 0 && degree <= 180){
        pulse_width = (degree * 11.11111) + 500;
        set_servo_pulsewidth(pi, gpio, pulse_width);
        printf("degree: %d\n", degree);
    }

	pigpio_stop(pi);

	return 0;
}
