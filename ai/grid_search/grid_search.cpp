#include "grid_search.hpp"
#include <cstdlib>

bool avanzar(vector<float> &params, int inf_limit, int sup_limit, float step, uint param_init, uint param_fin){
    for (size_t i = param_init; i < param_fin; i++) {
      if (params[i] >= sup_limit) {
        params[i] = inf_limit;
      }else{
        params[i] += step;
        return true;
      }
    }
    return false;
}

void grid_search::thorough_train(int inf_limit, int sup_limit, float step){
    //Precondiciones, pueden cambiar
    // assert(inf_limit < 0);
    // assert(sup_limit > 0);
    // assert(sup_limit + inf_limit == 0);
    // assert(step > 0);
    this->optimal_fitness = 0;

    pesos params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    do{
        float result = this->fitness(params);
        if (this->optimal_fitness <= result){
            this->optimal_fitness = result;
            this->optimal_weights = params;
            cout << "NUEVO MAX " << this->optimal_fitness << endl;
            for (uint i=0; i<this->optimal_weights.size();i++){
                cout << this->optimal_weights[i] << "\t ";
            }
            cout << endl << endl << endl;
            if (this->optimal_fitness>0.99) return;
        }

    } while (avanzar(params, inf_limit, sup_limit, step, 0, (uint)params.size()));
}

void grid_search::minecraft_train(int inf_limit, int sup_limit, float step){
    this->optimal_fitness = 0;

    pesos optimos_parciales((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    // param_init=0                     param_fin=params.size() - this->c
    pesos params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    uint param_init=0;
    uint param_fin=params.size() - this->c;

    //Tomo rarndom los ultimos valores
    for (uint i= param_fin; i< params.size(); i++){
        float randnum = inf_limit + static_cast <float> ((sup_limit-inf_limit)*rand()) /( static_cast <float> (RAND_MAX));
        params[i] = randnum;
    }
    do{
        float result = this->fitness(params);
        if (this->optimal_fitness <= result){
            this->optimal_fitness = result;
            this->optimal_weights = params;
            cout << "NUEVO MAX " << this->optimal_fitness << endl;
            for (uint i=0; i<this->optimal_weights.size();i++){
                cout << this->optimal_weights[i] << "\t ";
            }
            cout << endl << endl << endl;
            if (this->optimal_fitness>0.99) return;
        }
    } while (avanzar(params, inf_limit, sup_limit, step, param_init, param_fin));

    for (uint i = param_init; i < param_fin; i++) {
        optimos_parciales[i] = this->optimal_weights[i];
    }



    // param_init=params.size() - this->c      param_fin=params.size()
    params = pesos((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    param_init=params.size() - this->c;
    param_fin=params.size();

    //Tomo rarndom los ultimos valores
    for (uint i= 0; i< param_init; i++){
        float randnum = inf_limit + static_cast <float> ((sup_limit-inf_limit)*rand()) /( static_cast <float> (RAND_MAX));
        params[i] = randnum;
    }
    do{
        float result = this->fitness(params);
        if (this->optimal_fitness <= result){
            this->optimal_fitness = result;
            this->optimal_weights = params;
            cout << "NUEVO MAX " << this->optimal_fitness << endl;
            for (uint i=0; i<this->optimal_weights.size();i++){
                cout << this->optimal_weights[i] << "\t ";
            }
            cout << endl << endl << endl;
            if (this->optimal_fitness>0.99) return;
        }
    } while (avanzar(params, inf_limit, sup_limit, step, param_init, param_fin));

    for (uint i = param_init; i < param_fin; i++) {
        optimos_parciales[i] = this->optimal_weights[i];
    }
    cout << "LOS OPTIMOS PARCIALES SON: " << endl;
    for (uint i=0; i<optimos_parciales.size();i++){
        cout << optimos_parciales[i] << "\t ";
    }
    cout << endl << endl << endl;




}


float grid_search::fitness(pesos& params){
    return regular_fitness(this->columnas, this->filas, this->c, this->p, params);
}
