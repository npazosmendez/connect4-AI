#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "../ai/grid_search/grid_search.hpp"

using namespace std;

/* AI posibles: */
#define CANT_STEPS_VICT 1
#define CANT_STEPS_EMP 2


#ifndef EXP
    #error "ERROR: EXP no definida"
#endif


int read_int();
float read_float();

int main(int argc, char const *argv[]) {
    srand (time(NULL));
    // Qué forma tiene el juego?
    #if EXP==CANT_STEPS_VICT
        float step = read_float();
        grid_search grid(7,6,4,40,-1000,1000,step);

        for (int i=0; i<200; i++){
            grid.random_busqueda_local_solo_victoria();
        }
    #elif EXP==CANT_STEPS_EMP
        float step = read_float();
        grid_search grid(7,6,4,40,-1000,1000,step);
        for (int i=0; i<200; i++){
            grid.random_busqueda_local_first_lose();
        }
    #endif


    return 0;
}


/////////////////////
//*  Auxiliares    */
/////////////////////

int read_int() {
    string msg;
    cin >> msg;
    return stoi(msg);
}

float read_float() {
    string msg;
    cin >> msg;
    return stoi(msg);
}
