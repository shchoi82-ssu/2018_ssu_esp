#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#define GPIO 13

int main()
{
	if(gpioInitialise() < 0){
		printf("pigpio initialisation failed.\n");
		exit(-1);
	}

	gpioSetMode(GPIO, PI_OUTPUT);
    printf("gpio:%d\n", GPIO);

	for(int i = 0 ; i < 10 ; i++){
		gpioWrite(GPIO, 1);
		printf("LED ON\n");
		gpioDelay(500000);
		gpioWrite(GPIO, 0);
		printf("LED OFF\n");
		gpioDelay(500000);
	}

	gpioTerminate();
	return 0;
}
