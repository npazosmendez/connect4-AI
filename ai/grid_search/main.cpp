#include <iostream>
#include <string>

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

    // Qué forma tiene el juego?
    int n = read_int();
    int m = read_int();
    int c = read_int();
    int p = read_int();

    // grid_search gridsearch(n,m,c,p,-1,1,0.25);
    // gridsearch.minecraft_train(-1,1,0.25);
    grid_search grid(n,m,c,p,-2,2,0.5);
    pesos a = grid.get_random_params();
    cout<< "RANDOM VECTOR " << endl;
    print(a);
    vector<golosa> list = grid.get_neighbors_golosos(a);
    for (uint i=0;i<list.size();i++){
        cout << "VECINO " << endl;
        print(list[i].ver_parametros());
        print(list[i].ver_pesos_lineas());
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
