CC = g++
FLAGS = -std=c++11 -ggdb -Wall -O3
LDFLAGS= -lpthread
OBJ = grid_search.o ../golosa.o ../c_linea.o ../fitness.o
EXECS = grid_search

all: $(EXECS)

# Ejecutables (AIs)

grid_search: main.cpp $(OBJ)
	make -C ../
	$(CC) $(FLAGS) $(OBJ) $< -o ../../bin/$@ $(LDFLAGS)

# Objetos

%.o: %.cpp %.hpp
	$(CC) $(FLAGS) -c $< -o $@ $(LDFLAGS)


clean:
	rm -f *.o
	rm -f $(EXECS)
	rm -f *.gch
	-rm ../../bin/$(EXECS)
