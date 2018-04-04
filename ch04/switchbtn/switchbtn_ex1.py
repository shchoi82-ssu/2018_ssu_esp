#!/usr/bin/python3
import pigpio
import time

GPIO = 23

pi = pigpio.pi()
if not pi.connected:
    exit()

pi.set_mode(GPIO, pigpio.INPUT)
pi.set_pull_up_down(GPIO, pigpio.PUD_DOWN)

while True:
    level = pi.read(GPIO)
    print("{} {}".format("--__--" if (level == 1) else "------", level))
    time.sleep(0.1)
pi.stop()
