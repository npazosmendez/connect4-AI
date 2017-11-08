#ifndef GRID_SEARCH_H
#define GRID_SEARCH_H

#include <cassert>

#include "../golosa.hpp"
#include "../fitness.hpp"

using namespace std;
typedef vector<float> pesos;

class grid_search{
    public:
        grid_search(uint _columnas, uint _filas, uint _c, uint _p, float _inf, float _sup, float _step) :
            columnas(_columnas), filas(_filas), c(_c), p(_p), inf_limit(_inf), sup_limit(_sup), step(_step){
                // cout << _columnas << ", " << _filas << ", " << _c << ", " << _p << '\n';
                // cout << _inf << ", " << _sup << ", " << _step << '\n';
        };
        void thorough_train(int inf_limit, int sup_limit, float step);
        void minecraft_train(int inf_limit, int sup_limit, float step);
        void randomized_train();
        pesos get_random_params();
        list<golosa> get_neighbors_golosos(const pesos& params);
        vector<golosa> get_neighbors_golosos_v(const pesos& params);
        golosa random_busqueda_local_con_fixture();
        golosa random_busqueda_local_solo_victoria();
        golosa random_busqueda_local_first_lose();

    private:
        uint columnas,filas,c,p; //Caracteristicas del tablero a utilizar
        float inf_limit;
        float sup_limit;
        float step;
        bool fitness(pesos& params);
        // bool fitness(pesos& params, pesos& maximo);



};

#endif
