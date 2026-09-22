CC=g++
CFLAGS=-std=c++11 -Wall

all: main

main: main.cpp
	$(CC) $(CFLAGS) -o a.out main.cpp

clean:
	rm -f *.o a.out
