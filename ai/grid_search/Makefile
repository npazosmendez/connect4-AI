CC = g++
FLAGS = -std=c++11 -ggdb -Wall -O3
OBJ = grid_search.o ../golosa.o ../c_linea.o ../training.o
EXECS = grid_search

all: $(EXECS)

# Ejecutables (AIs)

grid_search: main.cpp $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $< -o ../../bin/$@

# Objetos

%.o: %.cpp %.hpp
	$(CC) $(FLAGS) -c $< -o $@


clean:
	rm -f *.o
	rm -f $(EXECS)
	rm -f *.gch
	-rm ../../bin/$(EXECS)
