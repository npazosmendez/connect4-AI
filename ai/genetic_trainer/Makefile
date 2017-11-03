CC= g++
CFLAGS= -std=c++11 -O3 -Wall
INCS= ../golosa
BIN= main

all: clean deps main

main:
	$(CC) $(CFLAGS) -o $@ $@.cpp

deps:
	make -C ..

clean:
	-rm -f *.o *~
	-rm -f $(AI_TYPES)

.PHONY:clean
