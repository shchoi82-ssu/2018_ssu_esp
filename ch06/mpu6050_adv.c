#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pigpiod_if2.h>

/*
typedef union accel_t_gyro_union
{
    struct
    {
        uint8_t x_accel_h;
        uint8_t x_accel_l;
        uint8_t y_accel_h;
        uint8_t y_accel_l;
        uint8_t z_accel_h;
        uint8_t z_accel_l;
        uint8_t t_h;
        uint8_t t_l;
        uint8_t x_gyro_h;
        uint8_t x_gyro_l;
        uint8_t y_gyro_h;
        uint8_t y_gyro_l;
        uint8_t z_gyro_h;
        uint8_t z_gyro_l;
    } reg;
    struct 
    {
        int x_accel;
        int y_accel;
        int z_accel;
        int temperature;
        int x_gyro;
        int y_gyro;
        int z_gyro;
    } value;
};
*/
// Use the following global variables and access functions to help store the overall
// rotation angle of the sensor
unsigned long last_read_time;
float         last_x_angle;  // These are the filtered angles
float         last_y_angle;
float         last_z_angle;  
float         last_gyro_x_angle;  // Store the gyro angles to compare drift
float         last_gyro_y_angle;
float         last_gyro_z_angle;

//  Use the following global variables and access functions
//  to calibrate the acceleration sensor
float    base_x_accel;
float    base_y_accel;
float    base_z_accel;

float    base_x_gyro;
float    base_y_gyro;
float    base_z_gyro;

void set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
    last_read_time = time;
    last_x_angle = x;
    last_y_angle = y;
    last_z_angle = z;
    last_gyro_x_angle = x_gyro;
    last_gyro_y_angle = y_gyro;
    last_gyro_z_angle = z_gyro;
}

unsigned long get_last_time() {return last_read_time;}
float get_last_x_angle() {return last_x_angle;}
float get_last_y_angle() {return last_y_angle;}
float get_last_z_angle() {return last_z_angle;}
float get_last_gyro_x_angle() {return last_gyro_x_angle;}
float get_last_gyro_y_angle() {return last_gyro_y_angle;}
float get_last_gyro_z_angle() {return last_gyro_z_angle;}


#define MPU6050_ADDRESS (0x68)
#define MPU6050_REG_PWR_MGMT_1 (0x6b)
#define MPU6050_REG_DATA_START (0x3b)
#define ACCEL_SCALE (16384.0)
#define GYRO_SCALE (131.0)


// The sensor should be motionless on a horizontal surface 
//  while calibration is happening
void calibrate_sensors(int pi, int fd) {
    int                   num_readings = 10;
    float                 x_accel = 0;
    float                 y_accel = 0;
    float                 z_accel = 0;
    float                 x_gyro = 0;
    float                 y_gyro = 0;
    float                 z_gyro = 0;

    for(int i = 0 ; i < num_readings ; i++) {
        uint8_t msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START);
        uint8_t lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+1);
        short accel_x = msb << 8 | lsb;
        x_accel += accel_x;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+2);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+3);
        short accel_y = msb << 8 | lsb;
        y_accel += accel_y;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+4);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+5);
        short accel_z = msb << 8 | lsb;
        z_accel += accel_z;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+6);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+7);
        short temp = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+8);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+9);
        short gyro_x = msb << 8 | lsb;
        x_gyro += gyro_x;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+10);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+11);
        short gyro_y = msb << 8 | lsb;
        y_gyro += gyro_y;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+12);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+13);
        short gyro_z = msb << 8 | lsb;
        z_gyro += gyro_z;
        time_sleep(0.1); // milli second
    }
    x_accel /= num_readings;
    y_accel /= num_readings;
    z_accel /= num_readings;
    x_gyro /= num_readings;
    y_gyro /= num_readings;
    z_gyro /= num_readings;

    // Store the raw calibration values globally
    base_x_accel = x_accel;
    base_y_accel = y_accel;
    base_z_accel = z_accel;
    base_x_gyro = x_gyro;
    base_y_gyro = y_gyro;
    base_z_gyro = z_gyro;

    //Serial.println("Finishing Calibration");
}



// wiringPi default baudrate 100Kbps
// $ gpio load i2c 1000 (kbps)
// will set the baud rate to 1000Kbps – ie. 1,000,000 bps 
int main() {
    int pi;
    if((pi = pigpio_start(NULL, NULL)) < 0) {
        perror("pigpio_start");
        return 1;
    }

    printf("MPU6050 starting\n");

    int fd;
    if((fd = i2c_open(pi, 1, MPU6050_ADDRESS, 0)) < 0){
        perror("i2c_open");
        return 1;
    }
    setbuf(stdout, NULL);

    i2c_write_byte_data(pi, fd, MPU6050_REG_PWR_MGMT_1, 0);
    
    calibrate_sensors(pi, fd);

    set_last_read_angle_data(get_current_tick(pi)/1000, 0, 0, 0, 0, 0, 0);

    while(1) {
        uint8_t msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START);
        uint8_t lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+1);
        short accel_x = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+2);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+3);
        short accel_y = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+4);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+5);
        short accel_z = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+6);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+7);
        short temp = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+8);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+9);
        short gyro_x = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+10);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+11);
        short gyro_y = msb << 8 | lsb;

        msb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+12);
        lsb = i2c_read_byte_data(pi, fd, MPU6050_REG_DATA_START+13);
        short gyro_z = msb << 8 | lsb;

        unsigned long t_now = get_current_tick(pi)/1000;

        // Convert gyro values to degrees/sec
        float FS_SEL = 131;
        gyro_x = (gyro_x - base_x_gyro)/FS_SEL;
        gyro_y = (gyro_y - base_y_gyro)/FS_SEL;
        gyro_z = (gyro_z - base_z_gyro)/FS_SEL;

        //printf("accel_x=% 7d, accel_y=% 7d, accel_z=% 7d, gyro_x=% 7d, gyro_y=% 7d, gyro_z=% 7d\r", accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z);
        // Get angle values from accelerometer
        float RADIANS_TO_DEGREES = 180/3.14159;

        //  float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
        float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
        float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
        float accel_angle_z = 0;

        // Compute the (filtered) gyro angles
        float dt =(t_now - get_last_time())/1000.0;
        float gyro_angle_x = gyro_x*dt + get_last_x_angle();
        float gyro_angle_y = gyro_y*dt + get_last_y_angle();
        float gyro_angle_z = gyro_z*dt + get_last_z_angle();


        // Compute the drifting gyro angles
        float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
        float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
        float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();

        // Apply the complementary filter to figure out the change in angle - choice of alpha is
        // estimated now.  Alpha depends on the sampling rate...
        float alpha = 0.96;
        float angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
        float angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
        float angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle

        // Update the saved data with the latest values
        set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);

        printf("accelermeter angle    x:%4.0f y:%4.0f z:%4.0f  ", accel_angle_x, accel_angle_y, accel_angle_z);
        printf("unfiltered_gyro_angle x:%4.0f y:%4.0f z:%4.0f  ", unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z); 
        printf("filtered angle        x:%4.0f y:%4.0f z:%4.0f\r", angle_x, angle_y, angle_z); 

        time_sleep(0.000005);
    }

}
