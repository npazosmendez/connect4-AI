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
            columnas(_columnas), filas(_filas), c(_c), p(_p), optimal_fitness(0){
                // cout << _columnas << ", " << _filas << ", " << _c << ", " << _p << ", " << '\n';
        };
        void thorough_train(int inf_limit, int sup_limit, float step);
        void minecraft_train(int inf_limit, int sup_limit, float step);
        const pesos get_optimal_weigths() {return this->optimal_weights;}
        float get_optimal_fitness() {return this->optimal_fitness;}
    private:
        uint columnas,filas,c,p; //Caracteristicas del tablero a utilizar
        pesos optimal_weights;
        float optimal_fitness;

        float fitness(pesos& params);



};

#endif
