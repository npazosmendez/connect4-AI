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
    gridsearch.minecraft_train(0,1,0.20);
    vector<float> v = gridsearch.get_optimal_weigths();
    for (uint i=0; i<v.size();i++){
        cout << v[i] << ", ";
    }
    cout << endl << endl;

    cout << "fitness optimo " << gridsearch.get_optimal_fitness() << endl << endl;
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
