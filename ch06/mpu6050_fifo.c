#include <bcm2835.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <signal.h>
#define _DBGMSG
#include "dbgmsg.h"
#include "mpu6500.h"

#define MPU6500_I2C_ADDR          0x68
#define MPU6500_REG_PWR_MGMT_1    0x6b
#define MPU6500_REG_DATA_START    0x3b
#define MPU6500_REG_WHO_AM_I      0x75
#define MPU6500_REG_SMPLRT_DIV 	  0x19
#define MPU6500_REG_CONFIG        0x1A
#define MPU6500_REG_GYRO_CONFIG   0x1B
#define MPU6500_REG_ACCEL_CONFIG2 0x1D
#define MPU6500_REG_USER_CTRL     0x6A
#define MPU6500_REG_FIFO_COUNT_H  0x72
#define MPU6500_REG_FIFO_COUNT_L  0x73
#define MPU6500_REG_FIFO_R_W      0x74
#define MPU6500_REG_FIFO_EN       0x23
#define MPU6500_REG_INT_PIN_CFG   0x37
#define MPU6500_REG_INT_ENABLE    0x38
#define MPU6500_REG_INT_STATUS    0x3A

#define FIFO_LADLE_SIZE		120
//#define FIFO_LADLE_SIZE		30

static int g_smplrt_div = 0;

static int g_is_init = 0;
static void sigint_handler(int arg);
static int g_thread_stop_flag = 0;
int g_overflow_detect = 0;

static pthread_t tid2, tid1, tid3;

static int bcm2835_ret;

/**********************************************
 * I2C 통신용 버퍼
 **********************************************/
static uint8_t tbuf[2];
//static uint8_t rbuf[256];
static uint8_t rbuf[256];

/**********************************************
 * 데이터
 **********************************************/
//static struct timespec g_start_tick, g_end_tick;
static char *g_accel_gyro_data;
static uint64_t g_total_read;

MPU6500_STATISTICS calib;

static void init_calib()
{
   calib.amaxX = INT32_MIN;   calib.amaxY = INT32_MIN;   calib.amaxZ = INT32_MIN;
   calib.aminX = INT32_MAX;   calib.aminY = INT32_MAX;   calib.aminZ = INT32_MAX;
   calib.asumX = 0LL;   calib.asumY = 0LL;   calib.asumZ = 0LL;
   calib.aavgX = 0.0;   calib.aavgY = 0.0;   calib.aavgZ = 0.0;
   calib.adifX = 0;   calib.adifY = 0;   calib.adifZ = 0;

   calib.gmaxX = INT32_MIN;   calib.gmaxY = INT32_MIN;   calib.gmaxZ = INT32_MIN;
   calib.gminX = INT32_MAX;   calib.gminY = INT32_MAX;   calib.gminZ = INT32_MAX;
   calib.gsumX = 0LL;   calib.gsumY = 0LL;   calib.gsumZ = 0LL;
   calib.gavgX = 0.0;   calib.gavgY = 0.0;   calib.gavgZ = 0.0;
   calib.gdifX = 0;   calib.gdifY = 0;   calib.gdifZ = 0;
}
/********************************************************************************************/


int mpu6500_iscomplete()
{
    return g_thread_stop_flag;
}

static void bcm2835_i2c_reason_code(int arg)
{
    switch(arg){
        case BCM2835_I2C_REASON_OK:
            DBGMSGI("success\n");
            break;
        case BCM2835_I2C_REASON_ERROR_NACK:
            DBGMSGI("failure: Received a NACK\n");
            break;
        case BCM2835_I2C_REASON_ERROR_CLKT:
            DBGMSGI("failure: Received Clock Stretch Timeout\n");
            break;
        case BCM2835_I2C_REASON_ERROR_DATA:
            DBGMSGI("failure: Not all data is sent / received\n");
            break;
    }
}

// snd_htimestamp_t을 나노초단위로 convert 하는 함수 (long long 타입)
// input : snd_htimestamp_t(eg. struct timespec)
// output : nanoseconds(long long) since epoch 1970
long long to_nsec(struct timespec t)
{
    long long nsec;

    // overflow 가능성있는지 체크
    // time_t 4byte max : 2147483647, min : -2147483648
    // 2038년까지 표현 가능
    // long long 타입으로 전부 담을수 있는지
    // 584942419300 년까지 표현 가능
    // 9223372036854775807까지 가능 사실상 overflow 불가능
    // 1000000000LL을 곱하여도 214748364000000000 < 9223372036854775807
    // tv_sec가 max여도 overflow 되지 않음
    nsec = t.tv_sec * 1000000000LL;
    nsec += t.tv_nsec;

    return nsec;
}

// 두시간의 차이를 반환한다. 
// input : struct timespec x 2
// output : nanosecond
long long diff_time(struct timespec t1, struct timespec t2)
{
    long long nsec1, nsec2;

    nsec1 = to_nsec(t1);
    nsec2 = to_nsec(t2);

    return nsec1 - nsec2;
}


static void* measure_rate_thread(void *arg)
{
    DBGMSGI("measure_rate_thread start!");

	struct timespec stime, etime;

	uint64_t startNread, endNread;
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
	uint64_t total_read = (uint64_t)arg;

	while(total_read >= g_total_read){
		startNread = g_total_read;
		clock_gettime(CLOCK_MONOTONIC, &stime);
		usleep(500000);
		endNread = g_total_read;
		clock_gettime(CLOCK_MONOTONIC, &etime);
		//DBGMSGI("stime %lld", (long long)to_nsec(stime));
		//DBGMSGI("etime %lld", (long long)to_nsec(etime));

		float bytepersec = (endNread-startNread) * 1000000000LL / (double)diff_time(etime, stime);

		fprintf(stderr, "measure_sampling_rate:%f Hz, %f byte/s\n",
				bytepersec/2./6,    // ax ay az gx gy gz each 2byte
				bytepersec);
	}

    DBGMSGI("measure_rate_thread end!");
    return NULL;
}

static void* busy_reader_thread(void *arg)
{
    int buffer_max = (uint32_t)arg;
    if(buffer_max == 0) return (void*)NULL;

    DBGMSGI("buffer_max:%d", buffer_max);

    g_accel_gyro_data = (char*)calloc(buffer_max, 1);

    DBGMSGI("busy_reader_thread start");
//  const struct sched_param priority = {1};
//  sched_setscheduler(2, SCHED_FIFO, &priority);
//	mlockall(MCL_CURRENT);      // paging lock
 
    int first = 1;
    while(1){

        DBGMSGI("MPU6500_REG_FIFO_EN");
        tbuf[0] = MPU6500_REG_FIFO_EN;
        //tbuf[1] = (uint8_t)0b00001000;
        tbuf[1] = (uint8_t)0b01111000;
        if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
            DBGMSGI("bcm2835_i2c_write error");
        }

        // buffer full 
        // 1초에 48000byte(6byte(accelometer value) * 8kHz)가 들어온다
        // 512byte는 0.0106666666666667만에 채워짐 512/48000
        int16_t fifo_count;
        uint32_t offset = 0;
        uint8_t int_status;

        while(!g_thread_stop_flag){
        	//usleep(1);
			//nanosleep(1);

            /**********************************************
             * INT_STATUS 
             **********************************************/
            tbuf[0] = MPU6500_REG_INT_STATUS;
            rbuf[0] = 0;
            bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1);
            int_status = rbuf[0];
            /**********************************************
             * fifo overflow check
             **********************************************/
//            DBGMSGI(("int_status:%X", (uint8_t)int_status);
            if((int_status & 0b00010000) != 0){
                DBGMSG("fifo overflow!!!!\ttry recovery");

                /**********************************************
                 * FIFO_EN disable
                 **********************************************/
                DBGMSGI("MPU6500_REG_FIFO_EN disable");
                tbuf[0] = MPU6500_REG_FIFO_EN;
                tbuf[1] = (uint8_t)0b00000000;
                if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
                    DBGMSGI("bcm2835_i2c_write error");
                }

                /**********************************************
                 * MPU6500_REG_USER_CTRL
                 **********************************************/
                /*
                 * 1 – Enable FIFO operation mode.
                 * 0 – Disable FIFO access from serial interface. To disable FIFO writes by DMA, use FIFO_EN register. To disable possible FIFO writes from DMP, disable the DMP.
                 */
                DBGMSGI("MPU6500_REG_USER_CTRL fifo reset");
                tbuf[0] = MPU6500_REG_USER_CTRL;
                tbuf[1] = (uint8_t)0b01000100;
                if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
                    DBGMSGI("bcm2835_i2c_write error");
                }
                fifo_count = 0;
                offset = 0;

                /**********************************************
                 * INT_STATUS 
                 **********************************************/
                tbuf[0] = MPU6500_REG_INT_STATUS;
                rbuf[0] = 0;
                bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1);
                int_status = rbuf[0];
                /**********************************************
                 * fifo overflow check
                 **********************************************/
                DBGMSGI("int_status:%X", (uint8_t)int_status);
                //bcm2835_delayMicroseconds(1000000);
                break;
            }
            /**********************************************
             * data ready check
             **********************************************/
            if((int_status & 0b00000001) == 0){
                //DBGMSGI("not ready");
                continue;
            }

            /********************************************************/
            // fifo count check
            /********************************************************/
            bcm2835_delayMicroseconds(1);
            tbuf[0] = MPU6500_REG_FIFO_COUNT_H;
            if(BCM2835_I2C_REASON_OK != (bcm2835_ret = bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1))){
                DBGMSGI("\rbcm2835_i2c_read_register_rs 1 error\t\t");
                bcm2835_i2c_reason_code(bcm2835_ret);
                break;
            }
            fifo_count = rbuf[0] << 8;

            //       bcm2835_delayMicroseconds(1);
            tbuf[0] = MPU6500_REG_FIFO_COUNT_L;
            if(BCM2835_I2C_REASON_OK != (bcm2835_ret = bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1))){
                DBGMSGI("\nbcm2835_i2c_read_register_rs 2 error\t\t");
                bcm2835_i2c_reason_code(bcm2835_ret);
                break;
            }
            fifo_count |= rbuf[0];
            //DBGMSGI("fifo_count:%10d\t", fifo_count);
            /********************************************************/


            /********************************************************/
            if(fifo_count > FIFO_LADLE_SIZE){
                tbuf[0] = MPU6500_REG_FIFO_R_W;
                if(BCM2835_I2C_REASON_OK != (bcm2835_ret = bcm2835_i2c_write((const char*)tbuf, 1))){
                    DBGMSGI("\nbcm2835_i2c_write FIFO_R_W error\t\t");
                    break;
                }
                if(BCM2835_I2C_REASON_OK != (bcm2835_ret = bcm2835_i2c_read((char*)rbuf, FIFO_LADLE_SIZE))){
                    DBGMSGI("\nbcm2835_i2c_read FIFO_R_W error\t\t");
                    break;
                }
                memcpy(g_accel_gyro_data + offset, rbuf, FIFO_LADLE_SIZE);
                offset += FIFO_LADLE_SIZE;
                g_total_read += FIFO_LADLE_SIZE;

                if(g_total_read >= (uint64_t)buffer_max){
                    if(g_total_read > (uint64_t)buffer_max){
                        DBGMSG("error! g_total_read > (uint64_t)buffer_max %lld %d", g_total_read, buffer_max);
                    }
                    memcpy(calib.raw_data, g_accel_gyro_data, g_total_read);
                    DBGMSGI("Data collection complete! total read byte:%lld", g_total_read);
                    return;
                }
            }
        }
    }
    DBGMSGI("busy_reader_thread end");

    g_thread_stop_flag = 1;

    return (void*)NULL;
}

void sigint_handler(int arg)
{
    DBGMSGI("\n!!!!!!!!!!!!!!!!sigint_handler start!!!!!!!!!!!!! arg:%d", arg);
    g_thread_stop_flag = 1;
    DBGMSGI("\n!!!!!!!!!!!!!!!!sigint_handler end!!!!!!!!!!!!!");
}

int mpu6500_init()
{
    // parse the command line
    if (!bcm2835_init()) {
        printf("bcm2835_init failed. Are you running as root??\n");
        goto error;
    }

    if (!bcm2835_i2c_begin()) {
        printf("bcm2835_i2c_begin failed. Are you running as root??\n");
        goto error;
    }

    /**********************************************
     * SIGINT Handler 
     **********************************************/
//    signal(SIGINT, sigint_handler);

    /**********************************************
     * SDA SCL PULL UP Register 
     **********************************************/
    bcm2835_gpio_set_pud(RPI_BPLUS_GPIO_J8_03, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_set_pud(RPI_BPLUS_GPIO_J8_05, BCM2835_GPIO_PUD_UP);

    /**********************************************/

    bcm2835_i2c_setSlaveAddress(MPU6500_I2C_ADDR);
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_148);
    DBGMSGI("i2c configuration Clock divider set to:%d", BCM2835_I2C_CLOCK_DIVIDER_148);
    //    DBGMSGI("len set to: %d\n", len);
    DBGMSGI("i2c configuration Slave address set to:%#x", MPU6500_I2C_ADDR);   

    /**********************************************
     * Device Address
     **********************************************/
    tbuf[0] = MPU6500_REG_WHO_AM_I;
    rbuf[0] = 0;
    bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1);
    DBGMSGI("WHO_AM_I status:%#04x", rbuf[0]);
    if(rbuf[0] != 0x68){
        DBGMSGE("WHO_AM_I error");
        goto error;
    }

    /**********************************************
     * MPU6500_REG_PWR_MGMT_1 RESET
     **********************************************/
    /*
     * Trigger a full device reset.
     * A small delay of ~50ms may be desirable after triggering a reset.
     * @see MPU6500_RA_PWR_MGMT_1
     * @see MPU6500_PWR1_DEVICE_RESET_BIT
     */
    tbuf[0] = MPU6500_REG_PWR_MGMT_1;
    tbuf[1] = (uint8_t)0b10000000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }
    DBGMSGI("MPU6500_REG_PWR_MGMT_1 reset :%#04x", tbuf[1]);
    bcm2835_delay(50);

    /**********************************************
     * MPU6500_REG_PWR_MGMT_1 CLKSEL Clock 
     **********************************************/
    tbuf[0] = MPU6500_REG_PWR_MGMT_1;
    rbuf[0] = 0;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1)){
        DBGMSGI("bcm2835_i2c_read_register_rs error");
        goto error;
    }
    DBGMSGI("PWR_MGMT_1:%#04x", rbuf[0]);

    /**********************************************
     * MPU6500_REG_PWR_MGMT_1 CLKSEL Clock 
     **********************************************/
    /*
     * An internal 8MHz oscillator, gyroscope based clock, or external sources can be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator or an external source is chosen as the clock source, the MPU-60X0 can operate in low power modes with the gyroscopes disabled.

     Upon power up, the MPU-60X0 clock source defaults to the internal oscillator. However, it is highly recommended that the device be configured to use one of the gyroscopes (or an external clock source) as the clock reference for improved stability. The clock source can be selected according to the following table:

     CLKSEL Clock Source
     0 Internal 8MHz oscillator
     1 PLL with X axis gyroscope reference
     2 PLL with Y axis gyroscope reference
     3 PLL with Z axis gyroscope reference
     4 PLL with external 32.768kHz reference
     5 PLL with external 19.2MHz reference
     6 Reserved
     7 Stops the clock and keeps the timing generator in reset
     */

    tbuf[0] = MPU6500_REG_PWR_MGMT_1;
    tbuf[1] = (uint8_t)0b00000001;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }
    DBGMSGI("MPU6500_REG_PWR_MGMT_1 CLKSEL Clock set %#04x", tbuf[1]);

    /**********************************************
     * MPU6500_REG_PWR_MGMT_1 CLKSEL Clock 
     **********************************************/
    tbuf[0] = MPU6500_REG_PWR_MGMT_1;
    rbuf[0] = 0;
    bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1);
    DBGMSGI("PWR_MGMT_1 status:%#04x", rbuf[0]);

    /**********************************************
     * MPU6500_REG_SMPLRT_DIV 
     **********************************************/
    /*
     * g_smplrt_div 설정
     * sampling rate
     */
    tbuf[0] = MPU6500_REG_SMPLRT_DIV;
//    tbuf[1] = (uint8_t)0b00000011;
    tbuf[1] = g_smplrt_div;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
    }
    DBGMSGI("MPU6500_REG_SMPRT_DIV set %#04x", g_smplrt_div);

    /**********************************************
     * MPU6500_REG_CONFIG CLKSEL Clock 
     **********************************************/
    tbuf[0] = MPU6500_REG_CONFIG;
    rbuf[0] = 0;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1)){
        DBGMSGI("bcm2835_i2c_read_register_rs error");
        goto error;
    }
    DBGMSGI("MPU6500_REG_CONFIG status:%#04x", rbuf[0]);

    //uint8_t DLPF_CFG = (uint8_t)0b000;
    uint8_t DLPF_CFG = 6;
    tbuf[0] = MPU6500_REG_CONFIG;
    tbuf[1] = (uint8_t)0b00000000 | (uint8_t)DLPF_CFG;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }
    DBGMSGI("MPU6500_REG_CONFIG Digital Low Pass Filter(DLPF) %#04x", tbuf[1]);

    /**********************************************
     * MPU6500_REG_GYRO_CONFIG 
     **********************************************/
    /*
     */	
    DBGMSGI("MPU6500_REG_GYRO_CONFIG");
    tbuf[0] = MPU6500_REG_GYRO_CONFIG;
    tbuf[1] = (uint8_t)0b00000000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }

    /**********************************************
     * MPU6500_REG_ACCEL_CONFIG2 
     **********************************************/
    /*
     * ACCEL_FCHOICE_B
     * Used to bypass DLPF as shown in the table below.
     * datasheet 15p
     * Accelerometer Data Rates and Bandwidth
     * 4kHz  (uint8_t)0b00001000;
     */	
    DBGMSGI("MPU6500_REG_ACCEL_CONFIG2");
    tbuf[0] = MPU6500_REG_ACCEL_CONFIG2;
    //tbuf[1] = (uint8_t)0b00001000;
    tbuf[1] = 6;
    //tbuf[1] = (uint8_t)0b00000000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }


    /**********************************************
     * MPU6500_REG_USER_CTRL
     **********************************************/
    /*
     * 1 – Enable FIFO operation mode.
     * 0 – Disable FIFO access from serial interface. To disable FIFO writes by DMA, use FIFO_EN register. To disable possible FIFO writes from DMP, disable the DMP.
     */
    DBGMSGI("MPU6500_REG_USER_CTRL");
    tbuf[0] = MPU6500_REG_USER_CTRL;
    tbuf[1] = (uint8_t)0b01000000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }


    /**********************************************
     * MPU6500_REG_FIFO_COUNT_H
     **********************************************/
    tbuf[0] = MPU6500_REG_FIFO_COUNT_H;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1)){
        DBGMSGI("bcm2835_i2c_read_register_rs error");
        goto error;
    }

    int16_t fifo_count = rbuf[0] << 8;

    tbuf[0] = MPU6500_REG_FIFO_COUNT_L;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_read_register_rs((char*)tbuf, (char*)rbuf, 1)){
        DBGMSGI("bcm2835_i2c_read_register_rs error");
        goto error;
    }

    fifo_count |= rbuf[0];

    DBGMSGI("MPU6500_REG_FIFO_COUNT count:%d", fifo_count);

    /**********************************************
     * MPU6500_REG_INT_PIN_CFB
     **********************************************/
    /*
     * 
     *
     */
    DBGMSGI("MPU6500_REG_INT_PIN_CFG");
    tbuf[0] = MPU6500_REG_INT_PIN_CFG;
    tbuf[1] = (uint8_t)0b00100000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }

    /**********************************************
     * MPU6500_REG_INT_ENABLE
     **********************************************/
    /*
     * 
     *
     */
    DBGMSGI("MPU6500_REG_INT_ENABLE");
    tbuf[0] = MPU6500_REG_INT_ENABLE;
    tbuf[1] = (uint8_t)0b00010000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
        goto error;
    }

    /**********************************************
     * Buffer Overflow Interrup handler
     **********************************************/
//    bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_29, BCM2835_GPIO_FSEL_INPT);
//    bcm2835_gpio_set_pud(RPI_BPLUS_GPIO_J8_29, BCM2835_GPIO_PUD_DOWN);

    /**********************************************
     * Buffer Overflow Check LED
     **********************************************/
//    bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_22, BCM2835_GPIO_FSEL_OUTP);
  //  bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_22, LOW);
    /*
     * 시스템이 중단됨...
     */
    //    bcm2835_gpio_hen(RPI_BPLUS_GPIO_J8_29);
    //    bcm2835_gpio_ren(RPI_BPLUS_GPIO_J8_29);
    //    bcm2835_gpio_aren(RPI_BPLUS_GPIO_J8_29);

    /********************************************************************************************/
    DBGMSGE("mpu6500_init success");
    return g_is_init = 1;

error:
    DBGMSGE("mpu6500_init faile");
    return g_is_init = 0;
}

void mpu6500_release()
{
    DBGMSGI("mpu6500_release");

    if(!g_is_init)
        return;
    /**********************************************
     * realease
     **********************************************/
    DBGMSGI("MPU6500_REG_FIFO_EN off");
    tbuf[0] = MPU6500_REG_FIFO_EN;
    tbuf[1] = (uint8_t)0b00000000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
    }
    bcm2835_delayMicroseconds(1);

    DBGMSGI("MPU6500_REG_INT_ENABLE off");
    tbuf[0] = MPU6500_REG_INT_ENABLE;
    tbuf[1] = (uint8_t)0b00000000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
    }
    bcm2835_delay(50);

/*
    DBGMSGI("try tid1 stop\n");
    g_thread_stop_flag = 1;
    bcm2835_delay(10);
    pthread_join(tid1, NULL);
*/

    DBGMSGI("try tid2 stop");
    g_thread_stop_flag = 1;
//    bcm2835_delay(10);
//    pthread_join(tid2, NULL);

/*
    while(1){
        g_thread_stop_flag = 1;
        if(pthread_tryjoin_np(tid1, NULL) == 0)
            break;
        bcm2835_delay(1000);
        DBGMSGI("waiting 1\n");
    }

    wcwinahile(1){
        g_thread_stop_flag = 1;
        if(pthread_tryjoin_np(tid2, NULL) == 0)
            break;
        bcm2835_delay(1000);
        DBGMSGI("waiting 2\n");
    }
*/

    DBGMSGI("MPU6500_REG_PWR_MGMT_1 reset");
    tbuf[0] = MPU6500_REG_PWR_MGMT_1;
    tbuf[1] = (uint8_t)0b10000000;
    if(BCM2835_I2C_REASON_OK != bcm2835_i2c_write((const char*)tbuf, 2)){
        DBGMSGI("bcm2835_i2c_write error");
    }

    // This I2C end is done after a transfer if specified
    bcm2835_i2c_end();   
    bcm2835_close();
	free(g_accel_gyro_data);
    g_accel_gyro_data = NULL;
    g_total_read = 0;
    g_thread_stop_flag = 0;
    g_overflow_detect = 0;
    g_is_init = 0;

}
#define DIVIDER 12
void mpu6500_save_result(FILE *out)
{
//    DBGMSGI("diff:%09lld %f s\n", diff_time(g_end_tick, g_start_tick), (double)diff_time(g_end_tick, g_start_tick)/1000000000ll);

	fprintf(out, "accelX,accelY,accelZ\n");
	init_calib();
	for(uint64_t i = 0, j = 0 ; i < g_total_read ; i+=DIVIDER, j++){
		uint8_t accelXh = calib.raw_data[i];
		uint8_t accelXl = calib.raw_data[i+1];
		uint8_t accelYh = calib.raw_data[i+2];
		uint8_t accelYl = calib.raw_data[i+3];
		uint8_t accelZh = calib.raw_data[i+4];
		uint8_t accelZl = calib.raw_data[i+5];
		uint8_t gyroXh  = calib.raw_data[i+6];
		uint8_t gyroXl  = calib.raw_data[i+7];
		uint8_t gyroYh  = calib.raw_data[i+8];
		uint8_t gyroYl  = calib.raw_data[i+9];
		uint8_t gyroZh  = calib.raw_data[i+10];
		uint8_t gyroZl  = calib.raw_data[i+11];

		int16_t accelX = accelXh << 8 | accelXl;
		int16_t accelY = accelYh << 8 | accelYl;
		int16_t accelZ = accelZh << 8 | accelZl;
		int16_t gyroX = gyroXh << 8 | gyroXl;
		int16_t gyroY = gyroYh << 8 | gyroYl;
		int16_t gyroZ = gyroZh << 8 | gyroZl;

		if(calib.amaxX < accelX) calib.amaxX = accelX;
		if(calib.aminX > accelX) calib.aminX = accelX;
		if(calib.amaxY < accelY) calib.amaxY = accelY;
		if(calib.aminY > accelY) calib.aminY = accelY;
		if(calib.amaxZ < accelZ) calib.amaxZ = accelZ;
		if(calib.aminZ > accelZ) calib.aminZ = accelZ;
		calib.asumX = calib.asumX + accelX;
		calib.asumY = calib.asumY + accelY;
		calib.asumZ = calib.asumZ + accelZ;

		if(calib.gmaxX < gyroX) calib.gmaxX = gyroX;
		if(calib.gminX > gyroX) calib.gminX = gyroX;
		if(calib.gmaxY < gyroY) calib.gmaxY = gyroY;
		if(calib.gminY > gyroY) calib.gminY = gyroY;
		if(calib.gmaxZ < gyroZ) calib.gmaxZ = gyroZ;
		if(calib.gminZ > gyroZ) calib.gminZ = gyroZ;
		calib.gsumX = calib.gsumX + gyroX;
		calib.gsumY = calib.gsumY + gyroY;
		calib.gsumZ = calib.gsumZ + gyroZ;
		//      calib.aavgX = (calib.aavgX * (int64_t)j + accelX) / (j+1);
		//      calib.aavgY = (calib.aavgY * (int64_t)j + accelY) / (j+1);
		//      calib.aavgZ = (calib.aavgZ * (int64_t)j + accelZ) / (j+1);

		//      DBGMSGI("[%020lld %05lld]%7d %7d %7d\n", i, j, accelX, accelY, accelZ);
		fprintf(out, "%d,%d,%d,%d,%d,%d\n", accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
	}
	calib.aavgX = calib.asumX / (int32_t)(g_total_read/DIVIDER);
	calib.aavgY = calib.asumY / (int32_t)(g_total_read/DIVIDER);
	calib.aavgZ = calib.asumZ / (int32_t)(g_total_read/DIVIDER);
	calib.adifX = calib.amaxX-calib.aminX;
	calib.adifY = calib.amaxY-calib.aminY;
	calib.adifZ = calib.amaxZ-calib.aminZ;

	calib.gavgX = calib.gsumX / (int32_t)(g_total_read/DIVIDER);
	calib.gavgY = calib.gsumY / (int32_t)(g_total_read/DIVIDER);
	calib.gavgZ = calib.gsumZ / (int32_t)(g_total_read/DIVIDER);
	calib.gdifX = calib.gmaxX-calib.gminX;
	calib.gdifY = calib.gmaxY-calib.gminY;
	calib.gdifZ = calib.gmaxZ-calib.gminZ;

	DBGMSGI("###################################################################");
	DBGMSGI("total:%lld samples", g_total_read/DIVIDER);
	DBGMSGI("amaxX:%-7d amaxY:%-7d amaxZ:%-7d", calib.amaxX, calib.amaxY, calib.amaxZ);
	DBGMSGI("aminX:%-7d aminY:%-7d aminZ:%-7d", calib.aminX, calib.aminY, calib.aminZ);
	DBGMSGI("aavgX:%-7.1f aavgY:%-7.1f aavgZ:%-7.1f", calib.aavgX, calib.aavgY, calib.aavgZ);
	DBGMSGI("adifX:%-7d adifY:%-7d adifZ:%-7d", calib.adifX, calib.adifY, calib.adifZ);
	DBGMSGI("asumX:%-lld asumY:%-lld asumZ:%-lld", calib.asumX, calib.asumY, calib.asumZ);
	DBGMSGI("###################################################################");
	DBGMSGI("gmaxX:%-7d gmaxY:%-7d gmaxZ:%-7d", calib.gmaxX, calib.gmaxY, calib.gmaxZ);
	DBGMSGI("gminX:%-7d gminY:%-7d gminZ:%-7d", calib.gminX, calib.gminY, calib.gminZ);
	DBGMSGI("gavgX:%-7.1f gavgY:%-7.1f gavgZ:%-7.1f", calib.gavgX, calib.gavgY, calib.gavgZ);
	DBGMSGI("gdifX:%-7d gdifY:%-7d gdifZ:%-7d", calib.gdifX, calib.gdifY, calib.gdifZ);
	DBGMSGI("gsumX:%-lld gsumY:%-lld gsumZ:%-lld", calib.gsumX, calib.gsumY, calib.gsumZ);
	DBGMSGI("###################################################################");
	//  DBGMSGI("test:%lld\n", calib.asumY);
	//  DBGMSGI("test:%lld\n", g_total_read/6);
	//  DBGMSGI("test:%lld\n", calib.asumY / (int32_t)(g_total_read/6));
	//  DBGMSGI("test:%lld\n", calib.asumY / 1000);
}

void mpu6500_start_sampling(int nbyte)
{
    //pthread_create(&tid1, NULL, interrupt_detect_thread, NULL);
    DBGMSGI("nbyte:%d", nbyte);
    pthread_create(&tid2, NULL, busy_reader_thread, (void*)nbyte);
    //pthread_create(&tid2, NULL, busy_reader_thread, 48000);

    pthread_join(tid2, NULL);
    //pthread_join(tid1, NULL);
}

void mpu6500_start_measure_rate(int nbyte)
{
    DBGMSGI("start measure rate");
    pthread_create(&tid3, NULL, measure_rate_thread, (void*)nbyte);
}

int main(int argc, char **argv) 
{
    if(argc == 2){
        g_smplrt_div = atoi(argv[1]);
    }

    int rate = 1000;
    int sample_size = 12;
    int sec = 3;
    int total_size = rate * sample_size * sec;

    mpu6500_init();

	mpu6500_start_measure_rate(total_size);

	mpu6500_start_sampling(total_size);

	mpu6500_save_result(stderr);

    mpu6500_release();
//   getchar();

    DBGMSGI("... done!");
    return 0;
}
