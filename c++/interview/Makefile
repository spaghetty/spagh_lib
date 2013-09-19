example: stringex array list base-ex stack-ex

stack-ex:
	g++ -std=c++0x -g -o stack_test examples/stack.cpp

stringex:
	g++ -std=c++0x -g -o string_test examples/string_ex.cpp


array:
	g++ -std=c++0x -g -o array_test examples/array.cpp	


list: #mylibs
	g++ -std=c++0x -Iinclude -g -Llibs -o list_test examples/list.cpp #libs/libsllist.a


base-ex:
	g++ -std=c++0x -Iinclude -g -o base examples/base.cpp
# mylibs:
# 	g++ -std=c++0x -Iinclude -g -c src/sllist.cpp -o libs/sllist.o
# 	ar rc libs/libsllist.a libs/sllist.o
