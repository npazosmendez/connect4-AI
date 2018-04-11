CC = g++
FLAGS = -std=c++11 -ggdb -Wall -O3
LDFLAGS= -lpthread
OBJ = ../golosa.o ../c_linea.o genetic_trainer.o ../fitness.o
EXECS = genetic_trainer

# Ejecutables (AIs)
genetic_trainer: main.cpp $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $< -o ../../bin/$@ $(LDFLAGS)

all:deps $(EXECS)

deps:
	make -C ../


# Objetos

%.o: %.cpp %.hpp
	$(CC) $(FLAGS) -c $< -o $@


clean:
	rm -f *.o
	rm -f $(EXECS)
	rm -f *.gch
	rm -f ../../bin/$(EXECS)
