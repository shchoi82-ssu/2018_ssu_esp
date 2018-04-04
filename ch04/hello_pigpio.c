#include <stdio.h>
#include <pigpio.h>

int main()
{
	int pigpio_version;

	pigpio_version = gpioVersion();
	printf("pigpio version : %d\n", pigpio_version);

	if(gpioInitialise() < 0) {
		printf("pigpio initialisation failed.\n");
	} else {
		printf("pigpio initialised okay.\n");
	}

	gpioTerminate();
	return 0;
}
