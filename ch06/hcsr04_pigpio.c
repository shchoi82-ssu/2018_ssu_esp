#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#define TRIG_PINNO 16
#define ECHO_PINNO 6

void trigger(void);
void cb_func_echo(int gpio, int level, uint32_t tick);

uint32_t start_tick_, dist_tick_;

int main()
{
    float distance;
    gpioCfgClock(2, 1, 1);
    if (gpioInitialise()<0) return 1;

    gpioSetMode(TRIG_PINNO, PI_OUTPUT);
    gpioSetMode(ECHO_PINNO, PI_INPUT);
    gpioSetAlertFunc(ECHO_PINNO, cb_func_echo);
    gpioWrite(TRIG_PINNO, PI_OFF);
    gpioDelay(1000000);     // delay 1 second

    printf("Raspberry Pi HC-SR04 UltraSonic sensor\n" );

    while(1){
        start_tick_ = dist_tick_ = 0;
        gpioTrigger(TRIG_PINNO, 10, PI_HIGH);
        gpioDelay(50000);

        if(dist_tick_ && start_tick_){
            //distance = (float)(dist_tick_) / 58.8235;
            distance = dist_tick_ / 1000000. * 340 / 2 * 100;
            if(distance < 2 || distance > 400)
                printf("range error\n");
            else
                printf("interval : %6dus, Distance : %6.1f cm\n", dist_tick_, distance);
        }
        else
            printf("sense error\n");

        gpioDelay(100000);
    }
    gpioTerminate();

    return 0;
}

void cb_func_echo(int gpio, int level, uint32_t tick)
{
    if(level == PI_HIGH)
        start_tick_ = tick;
    else if(level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}
