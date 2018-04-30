#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pigpiod_if2.h>


#define LOOP 1000
#define MPU6050_ADDRESS (0x68)
#define MPU6050_REG_PWR_MGMT_1 (0x6b)
#define MPU6050_REG_DATA_START (0x3b)
#define ACCEL_SCALE (16384.0)
#define GYRO_SCALE (131.0)

// wiringPi default baudrate 100Kbps
// $ gpio load i2c 1000 (kbps)
// will set the baud rate to 1000Kbps – ie. 1,000,000 bps 
int main() {
    uint32_t start_tick, diff_tick;
    printf("MPU6050 starting\n");
    // Setup Wiring Pi
    wiringPiSetup();

    // Open an I2C connection
    int fd;
    if((fd = wiringPiI2CSetup(MPU6050_ADDRESS)) < 0){
        perror("wiringPiI2CSetup");
        return 1;
    }
    setbuf(stdout, NULL);

    // Perform I2C work
    wiringPiI2CWriteReg8(fd, MPU6050_REG_PWR_MGMT_1, 0);

    start_tick = micros();
    for(int i = 0 ; i < LOOP ; i++) {
        uint8_t msb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START);
//        uint8_t lsb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+1);
//        short accelX = msb << 8 | lsb;
/*
        msb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+2);
        lsb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+3);
  //      short accelY = msb << 8 | lsb;

        msb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+4);
        lsb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+5);
    //    short accelZ = msb << 8 | lsb;

        msb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+6);
        lsb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+7);
      //  short temp = msb << 8 | lsb;

        msb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+8);
        lsb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+9);
//        short gyroX = msb << 8 | lsb;

        msb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+10);
        lsb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+11);
//        short gyroY = msb << 8 | lsb;

        msb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+12);
        lsb = wiringPiI2CReadReg8(fd, MPU6050_REG_DATA_START+13);
//        short gyroZ = msb << 8 | lsb;
*/
        //printf("accelX=% 7d, accelY=% 7d, accelZ=% 7d, gyroX=% 7d, gyroY=% 7d, gyroZ=% 7d\r", accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
    }
    diff_tick = micros() - start_tick;
    printf("\t%lld sps\n", 1000000LL * LOOP / diff_tick);
    return 0;
}
