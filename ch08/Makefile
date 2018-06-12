EXAMPLES = cJSON_build_ex1.out \
		   cJSON_build_ex2.out \
		   cJSON_parsing_ex.out \



all : $(EXAMPLES)

CFLAGS = -Wall -W

cJSON_build_ex1.out : cJSON_build_ex1.c
	gcc $(CFLAGS) $^ cJSON.c -o $@

cJSON_build_ex2.out : cJSON_build_ex2.c
	gcc $(CFLAGS) $^ cJSON.c -o $@

cJSON_parsing_ex.out : cJSON_parsing_ex.c
	gcc $(CFLAGS) $^ cJSON.c -o $@

clean : 
	-rm $(EXAMPLES)

