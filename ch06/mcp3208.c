#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

#define BAUD_RATE	1000000
#define LOOP		1000

int main(int argc, char* argv[])
{
    int pi;
    int spi;
    uint16_t value;
    uint64_t sum = 0;
    uint16_t avg;
    int channel = 0;
    char snd_buf[3];
    char rcv_buf[LOOP][3];
    uint32_t start_tick, diff_tick;

    if(argc == 2) channel = atoi(argv[1]);

    if((pi = pigpio_start(NULL, NULL)) < 0) {
        fprintf(stderr, "pigpio_start error\n"); 
        return 1;
    }

    if((spi = spi_open(pi, 0, BAUD_RATE, 0)) < 0) {
        fprintf(stderr, "spi_open error\n"); 
        return 2;
    }

    while(1){
        avg = sum = 0;
        snd_buf[0] = 0x18 | channel;

        start_tick = get_current_tick(pi);

        for(int i = 0 ; i < LOOP ; i++)
            spi_xfer(pi, spi, snd_buf, rcv_buf[i], 3);

        diff_tick = get_current_tick(pi) - start_tick;

        for(int i = 0 ; i < LOOP ; i++){
            value = ((rcv_buf[i][1] & 0x3f) << 8) | (rcv_buf[i][2] & 0xff);
            value = value >> 2;
            sum += value;
        }
        avg = sum / LOOP;

        printf("channel-%d : %4d %.1fv", channel, avg, 3.3*avg/4095);
        printf("\t%lld sps\n", 1000000LL * LOOP / diff_tick);
    }
	spi_close(pi, spi);
	pigpio_stop(pi);
	return 0;
}
