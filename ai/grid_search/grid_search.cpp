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

void randomize(pesos& params, int inf_limit, int sup_limit, uint idx_desde, uint idx_hasta) {
    for (uint i= idx_desde; i< idx_hasta; i++){
        float randnum = inf_limit + (sup_limit-inf_limit) * (static_cast <float> (rand()))/( static_cast <float> (RAND_MAX));
        params[i] = randnum;
    }
}

void grid_search::thorough_train(int inf_limit, int sup_limit, float step){
    //Precondiciones, pueden cambiar
    // assert(inf_limit < 0);
    // assert(sup_limit > 0);
    // assert(sup_limit + inf_limit == 0);
    // assert(step > 0);
    pesos params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    pesos optimal_weights = params;
    do{
        if (this->fitness(params, optimal_weights)){
            optimal_weights = params;
            cout << "NUEVO MAX " << endl;
            for (uint i=0; i<optimal_weights.size();i++){
                cout << optimal_weights[i] << "\t ";
            }
            cout << endl << endl << endl;
        }

    } while (avanzar(params, inf_limit, sup_limit, step, 1, (uint)params.size()));
}

void grid_search::minecraft_train(int inf_limit, int sup_limit, float step){

    pesos abs_optimos((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    pesos current_params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);

    // LA PRIMERA PARTE
    // param_init=0                     param_fin=params.size() - this->c

    uint param_init=1;
    uint param_fin=current_params.size() - this->c;

    //Tomo rarndom los ultimos valores
    randomize(current_params, inf_limit, sup_limit, param_fin, current_params.size());


    do{
        if (this->fitness(current_params, abs_optimos)){
            abs_optimos = current_params;
            cout << endl << endl << endl;
            cout << "--------------------------------------------------------NUEVO MAX ABSOLUTO" << "--------------------------------------------------------" << endl;
            for (uint i=0; i<abs_optimos.size(); i++){
                cout << abs_optimos[i] << "\t";
            }
            cout << endl << endl << endl;
        }
    } while (avanzar(current_params, inf_limit, sup_limit, step, param_init, param_fin));



    // LA SEGUNDA PARTE

    current_params = pesos((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);

    // PARA LOS PRIMEROS fijo los optimos parciales

    for (uint i = param_init; i < param_fin; i++ ){
        current_params[i] = abs_optimos[i];
    }


    // param_init=params.size() - this->c      param_fin=params.size()
    param_init=current_params.size() - this->c;
    param_fin=current_params.size();

    do{
        if (this->fitness(current_params, abs_optimos)){
            abs_optimos = current_params;
            cout << endl << endl << endl;
            cout << "--------------------------------------------------------NUEVO MAX ABSOLUTO" << "--------------------------------------------------------" << endl;
            for (uint i=0; i<abs_optimos.size(); i++){
                cout << abs_optimos[i] << "\t";
            }
            cout << endl << endl << endl;
        }
    } while (avanzar(current_params, inf_limit, sup_limit, step, param_init, param_fin));

    cout << "---------------------------------------------------------" << endl;
    cout << "                         RESUMEN                         " << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "Optimo Absoluto: " << endl;
    for (uint i=0; i<abs_optimos.size(); i++){
        cout << abs_optimos[i] << "\t";
    }
    cout << endl;
}


bool grid_search::fitness(pesos& params, pesos& maximo){
    return regular_fitness(this->columnas, this->filas, this->c, this->p, params, maximo);
}
