CC = g++
FLAGS = -std=c++11 -ggdb -Wall -O3

TESTS = test_golosa.cpp test_minimax.cpp test_c_linea.cpp test_training.cpp
GOOGLE_SHIT = -isystem googletest/include -pthread googletest/src/gtest_main.cc libgtest.a
OBJ = ../ai/golosa.o ../ai/c_linea.o ../ai/minimax_hc.o ../ai/minimax.o ../ai/minimax_ab.o
SRC = ../ai/*.cpp ../ai/*.hpp

all: clean tests

install:
	$(CC) -isystem googletest/include -Igoogletest -pthread -c googletest/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

tests: $(TESTS) $(SRC)
	make -C ../ai/
	$(CC) $(FLAGS) $(GOOGLE_SHIT) $(TESTS) $(OBJ) -o ../bin/tests

tests_c_linea: test_c_linea.cpp $(SRC)
	make -C ../ai/
	$(CC) $(FLAGS) $(GOOGLE_SHIT) test_c_linea.cpp $(OBJ) -o ../bin/tests_c_linea

test: all
	./tests

clean:
	rm -f *.o
	rm -f tests
	rm -f *.gch
	-rm ../bin/tests ../bin/tests_c_linea
