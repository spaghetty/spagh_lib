create__fnv= \
 if [ ! -e ./fnv ]; then ln -s ../../fnv ./fnv; fi

remove__fnv= \
 if [ -e ./fnv ]; then rm fnv; fi

all: prepare map_test

clean: remove
	rm map_test

prepare:
	$(create__fnv)

remove:
	$(remove__fnv)

map_test: fnv/fnv.c hmap.c main.c
	$(CC) -ggdb -O3 $^ -o map_test	
