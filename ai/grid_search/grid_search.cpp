#include "grid_search.hpp"

void grid_search::thorough_train(int inf_limit, int sup_limit, float step){
    //Precondiciones, pueden cambiar
    assert(inf_limit < 0);
    assert(sup_limit > 0);
    assert(sup_limit + inf_limit == 0);
    assert(step > 0);

    pesos params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    params[0] = (this->c)/2;
    for(int i = inf_limit; i<= sup_limit; i+=step){
        params[1] = i;
        int fitness_act = this->fitness(params);
        if (fitness_act >= this->optimal_fitness){
            this->optimal_fitness = fitness_act;
            this->optimal_weights = params;
        }
    }


}


float grid_search::fitness(pesos& params){
    return regular_fitness(this->columnas, this->filas, this->c, this->p, params, "../../");
}
