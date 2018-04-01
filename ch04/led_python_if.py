#!/usr/bin/python3
import pigpio
import time

GPIO = 13

pi = pigpio.pi()
if not pi.connected:
    exit()

pi.set_mode(GPIO, pigpio.OUTPUT)
print('gpio:', GPIO)

for i in range(5):
    pi.write(GPIO, 1)
    print('LED ON')
    time.sleep(1)
    pi.write(GPIO, 0)
    print('LED OFF')
    time.sleep(1)

pi.stop()
