#include <stdio.h>
#include <pigpiod_if2.h>

#define BAUD_RATE	1000000
#define CHANNEL		0
#define LOOP		100
#define LED_PINNO   21

int main(void)
{
	int pi;
	int spi;
	uint16_t value;
	uint64_t sum = 0;
	uint16_t avg;
	char snd_buf[3];
	char rcv_buf[LOOP][3];
	uint32_t start_tick, diff_tick;

	if((pi = pigpio_start(NULL, NULL)) < 0) {
		fprintf(stderr, "pigpio_start error\n"); 
		return 1;
	}

	if((spi = spi_open(pi, 0, BAUD_RATE, 0)) < 0) {
		fprintf(stderr, "spi_open error\n"); 
		return 2;
	}

    set_PWM_range(pi, LED_PINNO, 4095);

    while(1){
        avg = sum = 0;
        snd_buf[0] = 0x18 | CHANNEL;

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

        printf("channel-%d : %4d %.1fv", CHANNEL, avg, 3.3*avg/4095);
        printf("\t%lld sps\n", 1000000LL * LOOP / diff_tick);

        set_PWM_dutycycle(pi, LED_PINNO, avg);

    }
	spi_close(pi, spi);
	pigpio_stop(pi);
	return 0;
}
