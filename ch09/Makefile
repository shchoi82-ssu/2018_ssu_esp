EXAMPLES = thread_ex1.out \
		   thread_ex2.out \
		   thread_ex3.out \
		   thread_ex4.out \
		   thread_ex5.out \
		   thread_ex6.out \
		   thread_ex7.out \
		   thread_ex8.out \


all : $(EXAMPLES)

CFLAGS = -Wall -W -pthread

thread_ex1.out : thread_ex1.c
	gcc $(CFLAGS) $^ -o $@

thread_ex2.out : thread_ex2.c
	gcc $(CFLAGS) $^ -o $@

thread_ex3.out : thread_ex3.c
	gcc $(CFLAGS) $^ -o $@

thread_ex4.out : thread_ex4.c
	gcc $(CFLAGS) $^ -o $@

thread_ex5.out : thread_ex5.c
	gcc $(CFLAGS) $^ -o $@

thread_ex6.out : thread_ex6.c
	gcc $(CFLAGS) $^ -o $@

thread_ex7.out : thread_ex7.c
	gcc $(CFLAGS) $^ -o $@

thread_ex8.out : thread_ex8.c
	gcc $(CFLAGS) $^ -o $@

clean : 
	-rm $(EXAMPLES)

