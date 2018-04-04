#!/usr/bin/python3
import pigpio
import time

count = 0
def cbf(gpio, level, tick):
    global count
    count += 1
    print("count:", count)
    print("gpio:", gpio, " level:", level, " tick:", tick, "\n")

GPIO = 23

pi = pigpio.pi()
if not pi.connected:
    exit()

pi.set_mode(GPIO, pigpio.INPUT)
pi.set_pull_up_down(GPIO, pigpio.PUD_UP)

pi.callback(GPIO, pigpio.FALLING_EDGE, cbf)
input()

pi.stop()
