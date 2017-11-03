AI_DIR=ai
AI_SRC= ai.cpp
CC= g++
CFLAGS= -std=c++11 -O3 -Wall
INCS= c_linea
AI_TYPES= minimax minimax_ab golosa

all: clean $(AI_TYPES)

random_player:
	g++ -std=c++11 random_player.cpp -o random_player

## g++ -DAI=<ai> ...
## Por ejemplo: g++ -DAI=MINIMAX ai.cpp minimax.cpp -o minimax
## LOWER_VAR = `echo $(VAR) | tr A-Z a-z`
##%.o: %.cpp
##$(CC) $(CFLAGS) -o $@ -c $<

#minimax:$(INCS)
	#$(CC) $(CFLAGS) -DAI=$(shell echo $@ | tr a-z A-Z)\
		#$(AI_DIR)/$(AI_SRC) $(AI_DIR)/$@.cpp $(AI_DIR)/$<.o -o $@

#minimax_ab:$(INCS)
	#$(CC) $(CFLAGS) -DAI=$(shell echo $@ | tr a-z A-Z)\
		#$(AI_DIR)/$(AI_SRC) $(AI_DIR)/$@.cpp $(AI_DIR)/$<.o -o $@

#golosa:$(INCS)
	#$(CC) $(CFLAGS) -DAI=$(shell echo $@ | tr a-z A-Z)\
		#$(AI_DIR)/$(AI_SRC) $(AI_DIR)/$@.cpp $(AI_DIR)/$<.o -o $@

#$(INCS):
	#$(CC) $(CFLAGS) -o $(AI_DIR)/$@.o -c $(AI_DIR)/$@.cpp

clean:
	-rm -f *.o *~
	-rm -f $(AI_TYPES)

#tags:
	#-rm tags
	#ctags -R *.cpp

.PHONY:clean

