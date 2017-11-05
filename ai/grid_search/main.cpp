#include <iostream>
#include <string>

#include "grid_search.hpp"

using namespace std;

int read_int();

int main(int argc, char const *argv[]) {

    // Qué forma tiene el juego?
    int n = read_int();
    int m = read_int();
    int c = read_int();
    int p = read_int();

    grid_search gridsearch(n,m,c,p);
    gridsearch.minecraft_train(-1,1,0.25);
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
