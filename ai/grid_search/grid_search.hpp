#ifndef GRID_SEARCH_H
#define GRID_SEARCH_H

#include <cassert>

#include "../golosa.hpp"
#include "../fitness.hpp"

using namespace std;
typedef vector<float> pesos;

class grid_search{
    public:
        grid_search(uint _columnas, uint _filas, uint _c, uint _p) :
            columnas(_columnas), filas(_filas), c(_c), p(_p){
                // cout << _columnas << ", " << _filas << ", " << _c << ", " << _p << ", " << '\n';
        };
        void thorough_train(int inf_limit, int sup_limit, float step);
        void minecraft_train(int inf_limit, int sup_limit, float step);
    private:
        uint columnas,filas,c,p; //Caracteristicas del tablero a utilizar
        bool fitness(pesos& params, pesos& maximo);



};

#endif
