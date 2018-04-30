EXAMPLES =	am2302.out \
			hcsr04_pigpio.out \
			hcsr04_pigpiod_if2_async.out \
			hcsr04_mission1.out \
			hcsr04_mission2.out \
			hcsr04_mission3.out	\
			mcp3208.out \
			mcp3208_all.out \
			mcp3208_pigpio.out \
			mcp3208_mission.out \
			mpu6050.out \
			mpu6050_sps_pigpiod_if2.out \
			mpu6050_sps_pigpio.out \
			mpu6050_serial.out \
			mpu6050_wiringpi.out \
			mpu6050_adv.out \
			uart.out \
			uart_sender.out \
			uart_receiver.out \

all : $(EXAMPLES)

am2302.out : am2302.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

hcsr04_pigpio.out : hcsr04_pigpio.c
	gcc -Wall -W -pthread -lrt -lpigpio -Wno-unused-parameter $^ -o $@

hcsr04_pigpiod_if2_async.out : hcsr04_pigpiod_if2_async.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

hcsr04_mission1.out : hcsr04_mission1.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

hcsr04_mission2.out : hcsr04_mission2.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

hcsr04_mission3.out : hcsr04_mission3.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

mcp3208.out : mcp3208.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

mcp3208_all.out : mcp3208_all.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

mcp3208_pigpio.out : mcp3208_pigpio.c
	gcc -Wall -W -pthread -lrt -lpigpio -Wno-unused-parameter $^ -o $@

mcp3208_mission.out : mcp3208_mission.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

mpu6050.out : mpu6050.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

mpu6050_sps_pigpiod_if2.out : mpu6050_sps_pigpiod_if2.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -Wno-unused-parameter $^ -o $@

mpu6050_sps_pigpio.out : mpu6050_sps_pigpio.c
	gcc -Wall -W -pthread -lrt -lpigpio -Wno-unused-parameter $^ -o $@

mpu6050_wiringpi.out : mpu6050_wiringpi.c
	gcc -Wall -W -lwiringPi -Wno-unused-parameter $^ -o $@

mpu6050_serial.out : mpu6050_serial.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -lm -Wno-unused-parameter $^ -o $@

mpu6050_adv.out : mpu6050_adv.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -lm -Wno-unused-parameter $^ -o $@

uart.out : uart.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -lm -unused-parameter $^ -o $@

uart_sender.out : uart_sender.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -lm -Wno-unused-parameter $^ -o $@

uart_receiver.out : uart_receiver.c
	gcc -Wall -W -pthread -lrt -lpigpiod_if2 -lm -Wno-unused-parameter $^ -o $@


clean : 
	rm $(EXAMPLES)
