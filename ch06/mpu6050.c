#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <pigpiod_if2.h>

#define MPU6050_ADDRESS (0x68)
#define MPU6050_REG_PWR_MGMT_1 (0x6b)
#define MPU6050_REG_DATA_START (0x3b)

int main() {
    int pi;
    if((pi = pigpio_start(NULL, NULL)) < 0) return 1;

    printf("MPU6050 starting\n");

    int fd;
    if((fd = i2c_open(pi, 1, MPU6050_ADDRESS, 0)) < 0){
        perror("i2c_open");
        return 1;
    }
    setbuf(stdout, NULL);

    i2c_write_byte_data(pi, fd, MPU6050_REG_PWR_MGMT_1, 0);

    while(1) {
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

        printf("accelX=% 7d, accelY=% 7d, accelZ=% 7d, gyroX=% 7d, gyroY=% 7d, gyroZ=% 7d\r", accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
    }
    i2c_close(pi, fd);
    pigpio_stop(pi);

}
