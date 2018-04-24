EXAMPLES =	led_dimming_ex1.out \
			led_dimming_ex2.out \
			led_dimming_ex3.out \



all : $(EXAMPLES)  

led_dimming_ex1.out : led_dimming_ex1.c
	gcc -Wall -W -Wno-unused-parameter  -pthread -lpigpiod_if2 -lrt $^ -o $@

led_dimming_ex2.out : led_dimming_ex2.c
	gcc -Wall -W -Wno-unused-parameter  -pthread -lpigpiod_if2 -lrt $^ -o $@

led_dimming_ex3.out : led_dimming_ex3.c
	gcc -Wall -W -Wno-unused-parameter  -pthread -lpigpiod_if2 -lrt $^ -o $@


clean :
	rm $(EXAMPLES)
