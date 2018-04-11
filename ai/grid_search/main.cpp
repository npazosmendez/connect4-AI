#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "grid_search.hpp"

using namespace std;

int read_int();

void print(vector<float> a) {
    for (uint i=0; i<a.size();i++){
        cout << a[i] << '\t';
    }
    cout << endl;
}

int main(int argc, char const *argv[]) {
    srand (time(NULL));
    // Qué forma tiene el juego?
    int n = read_int();
    int m = read_int();
    int c = read_int();
    int p = read_int();

    grid_search grid(n,m,c,p,-1000,1000,100);

    // grid.thorough_train();
    grid.randomized_train();

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
