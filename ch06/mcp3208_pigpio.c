#include <stdio.h>
#include <pigpio.h>

#define BAUD_RATE	1000000
#define CHANNEL		0
#define LOOP		1000

int main(void)
{
	int spi;
	uint16_t value;
	uint64_t sum = 0;
	uint16_t avg;
	char snd_buf[3];
	char rcv_buf[LOOP][3];
	uint32_t start_tick, diff_tick;

	if(gpioInitialise() < 0) {
		fprintf(stderr, "pigpio_start error\n"); 
		return 1;
	}

	if((spi = spiOpen(0, BAUD_RATE, 0)) < 0) {
		fprintf(stderr, "spi_open error\n"); 
		return 2;
	}

    while(1){
        avg = sum = 0;
        snd_buf[0] = 0x18 | CHANNEL;

        start_tick = gpioTick();

        for(int i = 0 ; i < LOOP ; i++)
            spiXfer(spi, snd_buf, rcv_buf[i], 3);

        diff_tick = gpioTick() - start_tick;

        for(int i = 0 ; i < LOOP ; i++){
            value = ((rcv_buf[i][1] & 0x3f) << 8) | (rcv_buf[i][2] & 0xff);
            value = value >> 2;
            sum += value;
        }
        avg = sum / LOOP;

        printf("channel-%d : %4d %.1fv", CHANNEL, avg, 3.3*avg/4095);
        printf("\t%lld sps\n", 1000000LL * LOOP / diff_tick);
    }
	spiClose(spi);
	gpioTerminate();
	return 0;
}
