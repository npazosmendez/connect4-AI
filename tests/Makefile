CC = g++
FLAGS = -std=c++11 -ggdb -Wall -O3

TESTS = test_golosa.cpp
GOOGLE_SHIT = -isystem googletest/include -pthread googletest/src/gtest_main.cc libgtest.a
CODE = ../ai/golosa.cpp ../ai/golosa.hpp ../ai/c_linea.cpp ../ai/c_linea.hpp

all: clean tests

install:
	$(CC) -isystem googletest/include -Igoogletest -pthread -c googletest/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

tests:
	$(CC) $(FLAGS) $(GOOGLE_SHIT) $(TESTS) $(CODE) -o tests

test: all
	./tests

clean:
	rm -f *.o
	rm -f tests
	rm -f *.gch
