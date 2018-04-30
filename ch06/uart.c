#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

int stop_flag = 0;
void sig_handler(int signo)
{
    stop_flag = 1;
}

char tx_buf[256];
char rx_buf[256];
int nbyte;
int avail;
int main() {
    int pi;
    int tty_fd;

    signal(SIGINT, (void *)sig_handler);

    if((pi = pigpio_start(NULL, NULL)) < 0) {
        perror("pigpio_start");
        return 1;
    }

    if((tty_fd = serial_open(pi, "/dev/ttyAMA0", 115200, 0)) > 0) {
        fprintf(stderr, "serial_open error\n");
        return 2;
    }

    int avail;
    int idx;
    int len;
    while(!stop_flag) {
        idx = 0;
        fgets(tx_buf, sizeof(tx_buf), stdin);
        len = strlen(tx_buf) - 1;       // delete newline character 
        tx_buf[len] = '\0';
        serial_write(pi, tty_fd, tx_buf, len+1);
        time_sleep(0.1);

        while((avail = serial_data_available(pi, tty_fd)) > 0){
            rx_buf[idx] = serial_read_byte(pi, tty_fd);
            // printf("%c %d\n", rx_buf[idx], rx_buf[idx]);
            idx++;
        }
        printf("%s\n", rx_buf);
    }

    serial_close(pi, tty_fd);
    pigpio_stop(pi);
    return 1;
}
