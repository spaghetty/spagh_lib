all: fnv_test

clean: 
	rm fnv_test

fnv_test: fnv.c main.c
	$(CC) -ggdb -O3 $^ -o fnv_test	
