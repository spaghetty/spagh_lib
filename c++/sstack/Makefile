base: libsstack.a
	g++ -Iinclude -g -L. -o base_test examples/base.cpp libsstack.a

libsstack.a:
	g++ -Iinclude -g -c src/sstack.cpp -o sstack.o
	ar rc libsstack.a sstack.o

clean:
	rm *a
	rm *o