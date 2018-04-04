#include <stdio.h>
#include <pigpio.h>
#include "74hc595_functions.h" 

int pi;
int init(void)
{
    if((pi = gpioInitialise()) < 0){
        fprintf(stderr, "gpioInitialise error\n");
        return 0;
    }
    
    gpioSetMode(SDATA, PI_OUTPUT);
    gpioSetMode(STR_CLK, PI_OUTPUT);
    gpioSetMode(SHR_CLK, PI_OUTPUT);
    gpioSetMode(SHR_CLEAR, PI_OUTPUT);
    return 1;
}

void release(void)
{
    allclear();
    gpioTerminate();
}

void allclear(void)
{
    gpioWrite(SHR_CLEAR, 0);   gpioWrite(SHR_CLEAR, 1); // clear
    gpioWrite(STR_CLK, 0);     gpioWrite(STR_CLK, 1);   // latch
}

void set(int index)
{
    gpioWrite(SDATA, 1);
    gpioWrite(SHR_CLK, 0);
    gpioWrite(SHR_CLK, 1);
    gpioWrite(SDATA, 0);

    for(int i = 0 ; i < index ; i++){
        gpioWrite(SHR_CLK, 0);
        gpioWrite(SHR_CLK, 1);   // shift
    }
    gpioWrite(STR_CLK, 0);
    gpioWrite(STR_CLK, 1);   // latch
}

// unsinged 8bit int
void set8(uint8_t value)
{
	for(int i = 0 ; i < 8 ; i++){
		int mask = 0b1 << i;
		if((value & mask) == 0)
			gpioWrite(SDATA, 0);
		else
			gpioWrite(SDATA, 1);
		gpioWrite(SHR_CLK, 0); 
		gpioWrite(SHR_CLK, 1); 
	}
	// letch
	gpioWrite(STR_CLK, 0); 
	gpioWrite(STR_CLK, 1);
}

void set16(uint16_t value)
{
    for(int i = 0 ; i < 16 ; i++){
        int mask = 0b1 << i;
        if((value & mask) == 0)
            gpioWrite(SDATA, 0);
        else
            gpioWrite(SDATA, 1);
        gpioWrite(SHR_CLK, 0);
        gpioWrite(SHR_CLK, 1);
    }
    // letch
    gpioWrite(STR_CLK, 0);
    gpioWrite(STR_CLK, 1);
}


