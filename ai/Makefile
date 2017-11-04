CC = g++
FLAGS = -std=c++11 -ggdb -Wall -O3
OBJ = minimax.o golosa.o c_linea.o minimax_ab.o minimax_hc.o fitness.o
EXECS = minimax minimax_ab golosa minimax_hc

all: $(EXECS)

# Ejecutables (AIs)

minimax: ai.cpp $(OBJ)
	$(CC) $(FLAGS) -DAI=MINIMAX $(OBJ) $< -o ../bin/$@

minimax_ab: ai.cpp $(OBJ)
	$(CC) $(FLAGS) -DAI=MINIMAX_AB $(OBJ) $< -o ../bin/$@

minimax_hc: ai.cpp $(OBJ)
	$(CC) $(FLAGS) -DAI=MINIMAX_HC $(OBJ) $< -o ../bin/$@

golosa: ai.cpp $(OBJ) c_linea.o
	$(CC) $(FLAGS) -DAI=GOLOSA $(OBJ) $< -o ../bin/$@


# Objetos

%.o: %.cpp %.hpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f $(EXECS)
	-rm -f *.gch
	-rm ../bin/*
