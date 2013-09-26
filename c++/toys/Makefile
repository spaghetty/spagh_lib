example: tictactoe hanoi 4inarow

.PHONY: tictactoe hanoi 4inarow board.proto

board.proto: 
	protoc --proto_path=./resources --cpp_out=src resources/board.proto
	mv src/board.pb.h include

4inarow: board.proto
	g++ -std=c++0x -Iinclude -lPocoFoundation -lPocoNet -lboost_program_options -lprotobuf -g -o 4inarow src/fourinarow.cpp src/board.pb.cc

tictactoe:
	g++ -std=c++0x -lPocoFoundation -lPocoNet -lboost_program_options -lv8 -lpthread -g -o tictactoe src/tictactoe.cpp

hanoi:
	g++ -std=c++0x -g -o hanoi src/thehanoitowers.cpp
