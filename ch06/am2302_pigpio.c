#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int bit_count_;
int data_[5];
int start_tick_;

void callback(int gpio, int level, uint32_t tick);
int amg2303(float *out_humidity, float *out_temperature)
{
	int i;
//	int device = DEFAULT_DEVICE;
	int gpio = 4;
	int loopFlag = 1;

	gpioInitialise();

	// set callback function on gpio transition
	if (gpioSetAlertFunc(gpio, callback) != 0)
	{
		puts("PI_BAD_USER_GPIO");
		return -1;
	}

	// number of attempts to query sensor
	int tries = 3;

	// attempt to read sensor tries times or loop forever
		// start bit count less 3 non-data bits
		bit_count_ = -3;

		// zero the data array
		for (i = 0; i < 5; i++)
			data_[i] = 0;

		// set start time of first low signal
		start_tick_ = gpioTick();

		// set pin as output and make high for 50ms so we can detect first low
		gpioSetMode(gpio, PI_OUTPUT);
		for(int i = 0 ; i < 10 ; i++){
			gpioWrite(gpio, 1);
			usleep(20);
			gpioWrite(gpio, 0);
			usleep(20);
		}

		for(int i = 0 ; i < 10 ; i++){
			gpioWrite(gpio, 1);
			gpioDelay(1);
			gpioWrite(gpio, 0);
			gpioDelay(1);
		}

		gpioWrite(gpio, 1);
		gpioDelay(50000);

		// send start signal
		gpioWrite(gpio, 0);
		// wait for 18ms
		gpioDelay(18000);
		// return bus to high for 20us
		gpioWrite(gpio, 1);
		gpioDelay(20);
		// change to input mode
		gpioSetMode(gpio, PI_INPUT);

		// wait 50ms for data input
		gpioDelay(50000);

		// if we received 40 data bits and the checksum is valid
		if (bit_count_ == 40 &&
				data_[4] == ((data_[0] + data_[1] + data_[2] + data_[3]) & 0xff))
		{
			float tempC;
			float humidity;

			humidity = (data_[0] * 256 + data_[1]) / 10.0f;
			tempC = ((data_[2] & 0x7f) * 256 + data_[3]) / 10.0f;
			// check for negative temp bit
			if (data_[2] & 0x80)
				tempC *= -1.0f;
			// float tempF = 9.0f * tempC / 5.0f + 32.0f;
			// DMSG_DEBUG("Temperature: %.1fC %.1fF  Humidity: %.1f%%",
			//            tempC, tempF, humidity);
			// we're done
			tries = 0;

			*out_humidity = humidity;
			*out_temperature = tempC;
		}
		else
		{
			// DMSG_DEBUG("Data Invalid!");
			--tries;
		}

		// minimum device reset time, 2 seconds
		// gpioSleep(PI_TIME_RELATIVE, 2, 0);

	gpioTerminate();
	return 0;
}

// level change call back function
void callback(int gpio, int level, uint32_t tick)
{
	static int call_count;
	// if the level has gone low
	if (level == 0)
	{
		// duration is the elapsed time between lows
		int duration = tick - start_tick_;
		// set the timer start point to this low
		start_tick_ = tick;
		printf("%2d duration:%3d  ", ++call_count, duration);

		// if we have seen the first three lows which aren't data
		if (++bit_count_ > 0)
		{
			// point into data structure, eight bits per array element
			// shift the data one bit left
			data_[(bit_count_ - 1) / 8] <<= 1;
			// set data bit high if elapsed time greater than 100us
			data_[(bit_count_ - 1) / 8] |= (duration > 100 ? 1 : 0);
			printf("%d", (duration > 100 ? 1 : 0));
		}
		printf("\n");
	}
}

int main()
{
	float hum;
	float temp;
	amg2303(&hum, &temp);
	return 0;
}
