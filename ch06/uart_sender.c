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

    signal(SIGINT, (void *)sig_handler);

    if((pi = pigpio_start(NULL, NULL)) < 0) {
        perror("pigpio_start");
        return 1;
    }

    if((tty_fd = serial_open(pi, "/dev/ttyAMA0", 115200, 0)) > 0) {
        fprintf(stderr, "serial_open error\n");
        return 2;
    }
    
    strcpy(tx_buf, "You are connected to the pi3.\n\r");
    serial_write(pi, tty_fd, tx_buf, sizeof(tx_buf));

    int msg_cnt = 0;
    while(!stop_flag) {
        sprintf(tx_buf, "This is a message %d from pi\n\r", ++msg_cnt);
        serial_write(pi, tty_fd, tx_buf, sizeof(tx_buf));
        time_sleep(0.5);
    }
    strcpy(tx_buf, "You are disconnected to the pi3.\n\r");
    serial_write(pi, tty_fd, tx_buf, sizeof(tx_buf));

    serial_close(pi, tty_fd);
    pigpio_stop(pi);
    return 1;
}
