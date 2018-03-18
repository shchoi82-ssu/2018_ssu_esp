prog.out : main.o choi.o kim.o lee.o
	gcc -Wall -W main.o choi.o kim.o lee.o -o prog.out

main.o : main.c
	gcc -Wall -W -c main.c

choi.o : choi.c
	gcc -Wall -W -c choi.c

kim.o : kim.c
	gcc -Wall -W -c kim.c

lee.o : lee.c
	gcc -Wall -W -c lee.c

clean :
	rm *.o
	rm prog.out

