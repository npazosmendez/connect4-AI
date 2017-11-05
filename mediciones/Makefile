CC = g++

FLAGS = -std=c++11 -ggdb -Wall -O3

OBJ = ../ai/golosa.o ../ai/c_linea.o ../ai/minimax_hc.o ../ai/minimax.o ../ai/minimax_ab.o ../ai/fitness.o ../ai/genetic_trainer/genetic_trainer.o
SRC = ../ai/*.cpp ../ai/*.hpp

all: tiempos

tiempos: $(SRC) tiempos.cpp
	make -C ../ai/
	make -C ../ai/genetic_trainer/
	$(CC) $(FLAGS) $(OBJ) tiempos.cpp -o tiempos

victorias: $(SRC) victorias.cpp
	make -C ../ai/
	make -C ../ai/genetic_trainer/
	$(CC) $(FLAGS) $(OBJ) victorias.cpp -o tiempos

clean:
	rm -f *.o
	rm -f tiempos
	rm -f victorias
