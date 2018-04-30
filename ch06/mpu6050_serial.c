#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

#define MPU6050_ADDRESS (0x68)
#define MPU6050_REG_PWR_MGMT_1 (0x6b)
#define MPU6050_REG_DATA_START (0x3b)

int stop_flag=0;
void sig_handler(int signo)
{
    stop_flag=1;
}

int main() 
{
    int pi;
    int tty;
    int fd;
    char tx_buf[256];
    signal(SIGINT, (void *)sig_handler);
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "pigpio_start error\n");
        return 1;
    }

    if((tty = serial_open(pi, "/dev/ttyAMA0", 115200, 0))> 0){
        fprintf(stderr, "serial_open error\n");
        return 2;
    }

    if((fd = i2c_open(pi, 1, MPU6050_ADDRESS, 0)) < 0){
        perror("i2c_open");
        return 1;
    }

    i2c_write_byte_data(pi, fd, MPU6050_REG_PWR_MGMT_1, 0);

    while(!stop_flag) {
        uint8_t msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START);
        uint8_t lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+1);
        short accelX = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+2);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+3);
        short accelY = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+4);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+5);
        short accelZ = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+6);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+7);
        short temp = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+8);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+9);
        short gyroX = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+10);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+11);
        short gyroY = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+12);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+13);
        short gyroZ = msb << 8 | lsb;

        sprintf(tx_buf, "%d %d %d \r", accelX, accelY, accelZ);
        serial_write(pi, tty, tx_buf, strlen(tx_buf));
        time_sleep(0.009);
    }
    serial_close(pi, tty);
    i2c_close(pi, fd);
    pigpio_stop(pi);
    return 0;
}
