#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "../ai/grid_search/grid_search.hpp"

using namespace std;

int read_int();
float read_float();

int main(int argc, char const *argv[]) {
    srand (time(NULL));
    // Qué forma tiene el juego?

    float step = read_float();
    grid_search grid(7,6,4,40,-1000,1000,step);

    for (int i=0; i<200; i++){
        grid.random_busqueda_local_solo_victoria();
    }
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
