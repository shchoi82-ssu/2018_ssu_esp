#ifndef __MPU6500__
#define __MPU6500__
#include <stdio.h>

int mpu6500_init();
void mpu6500_release();
void mpu6500_save_result(FILE *out);
void mpu6500_start_sampling(int nbyte);
int mpu6500_iscomplete();

#define SAMPLING_RATE 1000              // SMPR divider, DLPF에따라서 변경됨
//#define SAMPLING_RATE 500              // SMPR divider, DLPF에따라서 변경됨
//#define MPU6500_DATA_SIZE SAMPLING_RATE * 12 / 2       // 0.5초
#define MPU6500_DATA_SIZE SAMPLING_RATE * 12 * 3       // 3초

typedef struct _MPU6500_STATISTICS{
    int32_t amaxX;    int32_t amaxY;    int32_t amaxZ;
    int32_t aminX;    int32_t aminY;    int32_t aminZ;
    int64_t asumX;    int64_t asumY;    int64_t asumZ;
    double aavgX;     double aavgY;     double aavgZ;
    int32_t adifX;    int32_t adifY;    int32_t adifZ;

    int32_t gmaxX;    int32_t gmaxY;    int32_t gmaxZ;
    int32_t gminX;    int32_t gminY;    int32_t gminZ;
    int64_t gsumX;    int64_t gsumY;    int64_t gsumZ;
    double gavgX;     double gavgY;     double gavgZ;
    int32_t gdifX;    int32_t gdifY;    int32_t gdifZ;
    uint8_t raw_data[MPU6500_DATA_SIZE];
} MPU6500_STATISTICS;

#endif // __MPU6500__

