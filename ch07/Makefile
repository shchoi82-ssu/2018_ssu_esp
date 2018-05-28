EXAMPLES = servo_ex1.out \
		   servo_ex2.out \
		   motor_ex1.out \
		   motor_ex2.out \
		   motor_ex3.out \
		   max7219_ex1.out \
		   max7219_ex2.out \
		   max7219_ex3.out \
		   terminalio.out \
		   noncanonical.out \
		   noncanonical_noecho.out \


CFLAGS = -Wall -W -Wno-unused-parameter
LIBS = -pthread -lrt -lpigpiod_if2

all : $(EXAMPLES)



servo_ex1.out : servo_ex1.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

servo_ex2.out : servo_ex2.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

motor_ex1.out : motor_ex1.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

motor_ex2.out : motor_ex2.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

motor_ex3.out : motor_ex3.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

max7219_ex1.out : max7219_ex1.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

max7219_ex2.out : max7219_ex2.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

max7219_ex3.out : max7219_ex3.c
	gcc $(CFLAGS) $(LIBS) $^ max7219_functions.c -o $@

terminalio.out : terminalio.c
	gcc $(CFLAGS) $^ -o $@

noncanonical.out : noncanonical.c
	gcc $(CFLAGS) $^ -o $@

noncanonical_noecho.out : noncanonical_noecho.c
	gcc $(CFLAGS) $^ -o $@

clean : 
	-rm $(EXAMPLES)

