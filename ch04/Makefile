EXAMPLES =	direct_reg_access.out \
			hello_pigpio.out \
			led_pigpio.out \
			led_pigpiod_if2.out \
			switchbtn_ex1.out \
			switchbtn_ex2.out \
			switchbtn_ex3.out \
			switchbtn_ex4.out \
			switchbtn_ex5.out \
			switchbtn_ex6.out \
			74hc595_ex1.out \
			74hc595_ex2.out \
			74hc595_ex3.out \
			74hc595_ex4.out \
			74hc595_ex5.out \
			74hc595_ex6.out \
			74hc595_ex7.out \
			74hc595_7seg_ex1.out \
			74hc595_7seg_ex2.out \
			74hc595_7seg_ex3.out \
			74hc595_dm_ex1.out \
			74hc595_dm_ex2.out \
			74hc595_dm_ex3.out \
			74hc595_dm_ex4.out \
			74hc595_dm_ex5.out \
			74hc595_dm_ex6.out \
			2dim_array_review.out \
			make_font_ex1.out \
			make_font_ex2.out \
			make_font_ex3.out \
			74hc595_dm_ex7.out \

CFLAGS = -Wall -W

all : $(EXAMPLES)  

direct_reg_access.out : direct_reg_access.c
	gcc $(CFLAGS) $^ -o $(OUTPUT_PATH)$@

hello_pigpio.out : hello_pigpio.c
	gcc $(CFLAGS) -lpigpio -lpthread -lrt $^ -o $@

led_pigpio.out : led_pigpio.c
	gcc $(CFLAGS) -lpigpio -lpthread -lrt $^ -o $@

led_pigpiod_if2.out : led_pigpiod_if2.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

switchbtn_ex1.out : switchbtn_ex1.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

switchbtn_ex2.out : switchbtn_ex2.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

switchbtn_ex3.out : switchbtn_ex3.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

switchbtn_ex4.out : switchbtn_ex4.c
	gcc $(CFLAGS) -Wno-unused-parameter -pthread -lpigpiod_if2 -lrt $^ -o $@

switchbtn_ex5.out : switchbtn_ex5.c
	gcc $(CFLAGS) -Wno-unused-parameter -pthread -lpigpiod_if2 -lrt $^ -o $@

switchbtn_ex6.out : switchbtn_ex6.c
	gcc $(CFLAGS) -Wno-unused-parameter -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_ex1.out : 74hc595_ex1.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_ex2.out : 74hc595_ex2.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_ex3.out : 74hc595_ex3.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_ex4.out : 74hc595_ex4.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_ex5.out : 74hc595_ex5.c 74hc595_functions.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_ex6.out : 74hc595_ex6.c 74hc595_functions.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_ex7.out : 74hc595_ex7.c 74hc595_functions.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_7seg_ex1.out : 74hc595_7seg_ex1.c 74hc595_functions.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_7seg_ex2.out : 74hc595_7seg_ex2.c 74hc595_functions.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_7seg_ex3.out : 74hc595_7seg_ex3.c 74hc595_functions.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_dm_ex1.out : 74hc595_dm_ex1.c 74hc595_functions.c
	gcc $(CFLAGS) -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_dm_ex2.out : 74hc595_dm_ex2.c 74hc595_functions.c
	gcc $(CFLAGS) -lpigpiod_if2 -pthread -lrt $^ -o $@

74hc595_dm_ex3.out : 74hc595_dm_ex3.c 74hc595_functions.c
	gcc $(CFLAGS) -Wno-overflow -pthread -lpigpiod_if2 -lrt $^ -o $@

74hc595_dm_ex4.out : 74hc595_dm_ex4.c 74hc595_functions_pigpio.c
	gcc $(CFLAGS) -Wno-overflow -pthread -lpigpio -lrt $^ -o $@

74hc595_dm_ex5.out : 74hc595_dm_ex5.c 74hc595_functions_pigpio.c
	gcc $(CFLAGS) -Wno-overflow -pthread -lpigpio -lrt $^ -o $@

74hc595_dm_ex6.out : 74hc595_dm_ex6.c 74hc595_functions_pigpio.c
	gcc $(CFLAGS) -Wno-overflow -pthread -lpigpio -lrt $^ -o $@

2dim_array_review.out : 2dim_array_review.c
	gcc $(CFLAGS) $^ -o $@

make_font_ex1.out : make_font_ex1.c
	gcc $(CFLAGS) $^ -o $@

make_font_ex2.out : make_font_ex2.c
	gcc $(CFLAGS) $^ -o $@

make_font_ex3.out : make_font_ex3.c
	gcc $(CFLAGS) $^ -o $@

74hc595_dm_ex7.out : 74hc595_dm_ex7.c 74hc595_functions_pigpio.c
	gcc $(CFLAGS) -Wno-overflow -pthread -lpigpio -lrt $^ -o $@

clean :
	rm $(EXAMPLES)
