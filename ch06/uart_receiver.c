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

int main() {
    int pi;
    int tty_fd;
    char tx_buf[256] = {0};
    char rx_buf[256] = {0};
    int idx = 0;
    int avail;

    signal(SIGINT, (void *)sig_handler);

    if((pi = pigpio_start(NULL, NULL)) < 0) {
        perror("pigpio_start");
        return 1;
    }

    if((tty_fd = serial_open(pi, "/dev/ttyAMA0", 115200, 0)) > 0) {
        fprintf(stderr, "serial_open error\n");
        return 2;
    }
    
    strcpy(tx_buf, "You are connected to the pi3.\n\rPlease input a message.\n\r");
    serial_write(pi, tty_fd, tx_buf, sizeof(tx_buf));

    while(!stop_flag) {
        idx = 0;
        while((avail = serial_data_available(pi, tty_fd)) > 0){
            rx_buf[idx] = serial_read_byte(pi, tty_fd);
            printf("%c %d\n", rx_buf[idx], rx_buf[idx]);
            idx++;
        }

    //    time_sleep(0.5);
    }
    strcpy(tx_buf, "You are disconnected to the pi3.\n\r");
    serial_write(pi, tty_fd, tx_buf, strlen(tx_buf));

    serial_close(pi, tty_fd);
    pigpio_stop(pi);
    return 1;
}
