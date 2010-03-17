all: dyn_test modules

clean: 
	rm dyn_test *lac

dyn_test:  mod_load.c main.c
	$(CC) -ggdb -ldl -O3 $^ -o dyn_test

modules: prova.c
	$(CC) -shared -fPIC $^ -o prova.lac