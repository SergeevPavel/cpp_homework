CC=g++
CPP_FLAGS=-Werror -Wall -g -std=c++11


all: bin/main

bin/main: bin/main.o
	$(CC) $(CPP_FLAGS) bin/main.o -o bin/main

bin/main.o: src/main.cpp src/linked_ptr.h
	$(CC) $(CPP_FLAGS) -c src/main.cpp -o bin/main.o

clean:
	rm -f bin/*

.PHONY: clean
