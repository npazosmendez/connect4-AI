AI_DIR=ai
AI_SRC= ai.cpp
CC= g++
CFLAGS= -std=c++11 -O3 -Wall
INCS= c_linea

all: clean random_player
	make -C ai/

random_player:
	g++ -std=c++11 random_player.cpp -o bin/random_player

clean:
	-rm -f bin/*
	make -C ai/ clean
	-rm -f *.o *.pyc *.gch *~
	-rm -f random_player

.PHONY:clean
