#include <pigpiod_if2.h>
#include <stdio.h>
#include <unistd.h>
#define SDAPIN 4

void cb_func(int pi, unsigned user_gpio, unsigned level, uint32_t tick);

int call_count_ = 0;
uint32_t start_tick_;
uint8_t data_[5];

int main()
{
    int pi;
    float tempC;
    float humidity;

    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "pigpio_start error\n");
        return 1;
    }


    set_mode(pi, SDAPIN, PI_OUTPUT);

    start_tick_ = get_current_tick(pi);
    gpio_write(pi, SDAPIN, 1);                      // init
    time_sleep(1);							
    callback(pi, SDAPIN, FALLING_EDGE, cb_func);    // call back function falling_edge

    gpio_write(pi,SDAPIN, 0);
    time_sleep(0.0008);                             // send start signal, at least 800us

    set_pull_up_down(pi, SDAPIN, PI_PUD_UP);        // pull up
    set_mode(pi, SDAPIN, PI_INPUT);                 // change mode

    usleep(10000);

    if (call_count_ == 43 && data_[4] == ((data_[0] + data_[1] + data_[2] + data_[3]) & 0xff)) {
        humidity = (data_[0] * 256 + data_[1]) / 10.0f;
        tempC = ((data_[2] & 0x7f) * 256 + data_[3]) / 10.0f;
        if (data_[2] & 0x80)
            tempC *= -1.0f;
    } else 
        printf("Data Invalid!\n");

    printf("Temperature: %.1fC Humidity: %.1f%%\n", tempC, humidity);
    pigpio_stop(pi);

    return 0;
}

void cb_func(int pi, unsigned user_gpio, unsigned level, uint32_t tick)
{
    static int data_bit_offset = 0;

    int duration = tick - start_tick_;
    start_tick_ = tick;
    call_count_++;

    printf("%2d duration:%3d  ", call_count_, duration);

    if(call_count_ == 1) {
        printf("init");
        data_bit_offset = 0;
    } else if(call_count_ == 2)
        printf("Tbe+Tgo");
    else if(call_count_ == 3)
        printf("Trel+Treh");
    else {
        data_[data_bit_offset / 8] <<= 1;      // shift
        data_[data_bit_offset / 8] |= (duration > 100 ? 1 : 0);
        printf("%d", (duration > 100 ? 1 : 0));
        data_bit_offset++;
    }
    printf("\n");
}
