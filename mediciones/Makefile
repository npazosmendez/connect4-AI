CC = g++

FLAGS = -std=c++11 -ggdb -Wall -O3

OBJ = ../ai/golosa.o ../ai/c_linea.o ../ai/minimax_hc.o ../ai/minimax.o ../ai/minimax_ab.o ../ai/fitness.o ../ai/genetic_trainer/genetic_trainer.o
OBJ_GRID = ../ai/c_linea.o ../ai/fitness.o ../ai/golosa.o ../ai/grid_search/grid_search.o
SRC = ../ai/*.cpp ../ai/*.hpp
SRC_GRID = ../ai/grid_search/*.cpp ../ai/grid_search/*.hpp

all: tiempos

tiempos: $(SRC) tiempos.cpp
	make -C ../ai/
	make -C ../ai/genetic_trainer/
	$(CC) $(FLAGS) $(OBJ) tiempos.cpp -o tiempos

victorias: $(SRC) victorias.cpp
	make -C ../ai/
	make -C ../ai/genetic_trainer/
	$(CC) $(FLAGS) $(OBJ) victorias.cpp -o tiempos

grid_steps_vict: $(SRC) $(SRC_GRID) grid.cpp
	make -C ../ai/
	make -C ../ai/grid_search/
	$(CC) -DEXP=CANT_STEPS_VICT $(FLAGS) $(OBJ_GRID) grid.cpp -o grid_steps_vict

grid_steps_emp: $(SRC) $(SRC_GRID) grid.cpp
	make -C ../ai/
	make -C ../ai/grid_search/
	$(CC) -DEXP=CANT_STEPS_EMP $(FLAGS) $(OBJ_GRID) grid.cpp -o grid_steps_emp


clean:
	rm -f *.o
	rm -f tiempos
	rm -f victorias
	rm -f grid_steps_emp
	rm -f grid_steps_vict
