#include <stdio.h>
#include <pigpiod_if2.h>

#define BAUD_RATE	1000000
#define TOTAL_CHAN	8
#define LOOP		100

int main(void)
{
    int pi;
    int spi;
    uint16_t value;
    uint64_t sum[TOTAL_CHAN];
    uint16_t avg[TOTAL_CHAN];
    char snd_buf[3];
    char rcv_buf[LOOP][TOTAL_CHAN][3];

    if((pi = pigpio_start(NULL, NULL)) < 0) {
        fprintf(stderr, "pigpio_start error\n"); 
        return 1;
    }

    if((spi = spi_open(pi, 0, BAUD_RATE, 0)) < 0) {
        fprintf(stderr, "spi_open error\n"); 
        return 2;
    }

    while(1){
        for(int j = 0 ; j < TOTAL_CHAN ; j++)
            avg[j] = sum[j] = 0;

        for(int i = 0 ; i < LOOP ; i++)
            for(int j = 0 ; j < TOTAL_CHAN ; j++){
                snd_buf[0] = 0x18 | j;
                spi_xfer(pi, spi, snd_buf, rcv_buf[i][j], 3);
            }

        for(int i = 0 ; i < LOOP ; i++){
            for(int j = 0 ; j < TOTAL_CHAN ; j++){
                value = ((rcv_buf[i][j][1] & 0x3f) << 8) | (rcv_buf[i][j][2] & 0xff);
                value = value >> 2;
                sum[j] += value;
            }
        }
        for(int j = 0 ; j < TOTAL_CHAN ; j++){
            avg[j] = sum[j] / LOOP;
            //            printf("%4d  %4.1fv  |  ", avg[j], 3.3 * avg[j] / 4095);
            printf("%4d(%3.1fv) | ", avg[j], 3.3 * avg[j] / 4095);
        }
        printf("\n");

    }
    spi_close(pi, spi);
    pigpio_stop(pi);
    return 0;
}
