#include "grid_search.hpp"
#include <cstdlib>
#include <cmath>
#include <list>
#include <algorithm>

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
    float optimal_fitness = 0;

    pesos params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), inf_limit);
    pesos optimal_weights = params;
    do{
        float result = this->fitness(params);
        if (optimal_fitness <= result){
            optimal_fitness = result;
            optimal_weights = params;
            cout << "NUEVO MAX " << result << endl;
            for (uint i=0; i<optimal_weights.size();i++){
                cout << optimal_weights[i] << "\t ";
            }
            cout << endl << endl << endl;
        }

    } while (avanzar(params, inf_limit, sup_limit, step, 1, (uint)params.size()));
}

bool grid_search::fitness(pesos& params){
    return regular_fitness(this->columnas, this->filas, this->c, this->p, params);
}

// bool grid_search::fitness(pesos& params, pesos& maximo){
//     return regular_fitness(this->columnas, this->filas, this->c, this->p, params, maximo);
// }




/************************************** EMPIEZO CON EL GRASP, LO DE ARRIBA ESTA MEDIO DESORDENADO **********************************************/



pesos grid_search::get_random_params() {

    pesos params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), this->inf_limit);

    //Elijo el 1er elem de esta lista
    vector<int> posibles_inicios;
    for (int i=-1; i<(int)this->columnas; i++){
        posibles_inicios.push_back(i);
    }

    params[0] = posibles_inicios[rand() % posibles_inicios.size()];

    //Elijo todo el resto de aca
    vector<float> posibles_pesos;
    for (float i = this->inf_limit; i <= this->sup_limit; i += this->step ){
        posibles_pesos.push_back(i);
    }

    for (uint i= 1; i< params.size(); i++){
        params[i] = posibles_pesos[rand() % posibles_pesos.size()];
    }

    return params;
}


list<golosa> grid_search::get_neighbors_golosos(const pesos& params){
    // Retorna los vecinos golosos

    list<golosa> golosos;
    if (params[0]> -1) {
        pesos copy = params;
        copy[0] = params[0] -1;
        golosa g = golosa(copy, this->columnas, this->filas, this->c);
        golosos.push_back(g);
    }
    if (params[0]<(int)this->columnas-1) {
        pesos copy = params;
        copy[0] = params[0] +1;
        golosa g = golosa(copy, this->columnas, this->filas, this->c);
        golosos.push_back(g);
    }

    for(uint i=1; i<params.size(); i++) {
        if (params[i] - this->step >= this->inf_limit) {
            pesos copy = params;
            copy[i] = params[i] - this->step;
            golosa g = golosa(copy, this->columnas, this->filas, this->c);
            golosos.push_back(g);
        }
        if (params[i] + this->step <= this->sup_limit) {
            pesos copy = params;
            copy[i] = params[i] +this->step;
            golosa g = golosa(copy, this->columnas, this->filas, this->c);
            golosos.push_back(g);
        }
    }
    // cout << "GOLOSO" << endl;
    // for (list<golosa>::iterator it = golosos.begin(); it != golosos.end(); ++it) {
    //     pesos params = (*it).join_params();
    //     for (uint i=0; i<params.size();i++){
    //         cout << params[i] << '\t';
    //     }
    //     cout << endl;
    // }
    return golosos;
}

vector<golosa> grid_search::get_neighbors_golosos_v(const pesos& params) {
    // Retorna los vecinos golosos

    vector<golosa> golosos;
    if (params[0]> -1) {
        pesos copy = params;
        copy[0] = params[0] -1;
        golosa g = golosa(copy, this->columnas, this->filas, this->c);
        golosos.push_back(g);
    }
    if (params[0]<(int)this->columnas-1) {
        pesos copy = params;
        copy[0] = params[0] +1;
        golosa g = golosa(copy, this->columnas, this->filas, this->c);
        golosos.push_back(g);
    }

    for(uint i=1; i<params.size(); i++) {
        if (params[i] - this->step >= this->inf_limit) {
            pesos copy = params;
            copy[i] = params[i] - this->step;
            golosa g = golosa(copy, this->columnas, this->filas, this->c);
            golosos.push_back(g);
        }
        if (params[i] + this->step <= this->sup_limit) {
            pesos copy = params;
            copy[i] = params[i] +this->step;
            golosa g = golosa(copy, this->columnas, this->filas, this->c);
            golosos.push_back(g);
        }
    }
    // cout << "GOLOSO" << endl;
    // for (list<golosa>::iterator it = golosos.begin(); it != golosos.end(); ++it) {
    //     pesos params = (*it).join_params();
    //     for (uint i=0; i<params.size();i++){
    //         cout << params[i] << '\t';
    //     }
    //     cout << endl;
    // }
    random_shuffle ( golosos.begin(), golosos.end() );
    return golosos;
}


golosa grid_search::random_busqueda_local_con_fixture(){
    pesos centro = this->get_random_params();
    pesos current_winner = centro;
    // cout << "RANDOM" << endl;
    // for (uint i=0; i<current_winner.size();i++){
    //     cout << current_winner[i] << '\t';
    // }
    // cout << endl;

    int jugados = 0;
    do {
        centro = current_winner;



        list<golosa> competidores = this->get_neighbors_golosos(centro);
        golosa g = golosa(centro, this->columnas, this->filas, this->c);
        competidores.push_back(g);

        // for (list<golosa>::iterator it = competidores.begin(); it != competidores.end(); ++it) {
        //     pesos params = (*it).join_params();
        //     cout << "Jugador" << endl;
        //     for (uint i=0; i<params.size();i++){
        //         cout << params[i] << '\t';
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        list<golosa> results = fixture_golosas(this->columnas, this->filas, this->c, this->p, competidores);
        current_winner = results.front().join_params();
        cout << "current_winner" << endl;
        for (uint i=0; i<current_winner.size();i++){
            cout << current_winner[i] << '\t';
        }
        cout << endl;
        jugados ++;
    } while (centro != current_winner);

    //Cuando se repite 2 veces el mismo campeon salimos
    // for (uint i=0; i<current_winner.size();i++){
    //     cout << current_winner[i] << '\t';
    // }
    // cout << endl;
    cout << "TRAS JUGAR " << jugados << " PARTIDOS" << endl;
    return golosa(current_winner, this->columnas, this->filas, this->c);
}

golosa grid_search::random_busqueda_local_solo_victoria() {
    pesos centro = this->get_random_params();
    pesos anterior = centro;
    pesos current_winner = centro;

    int jugados = 0;
    int ganados = 0;
    do {
        anterior = centro;
        centro = current_winner;
        vector<golosa> competidores = this->get_neighbors_golosos_v(centro);

        golosa g_centro = golosa(centro, this->columnas, this->filas, this->c);
        for (uint i = 0; i< competidores.size(); i++) {
            int result = ida_y_vuelta(this->columnas, this->filas, this->c, this->p, g_centro, competidores[i]);
            if (result == SEGUNDO){
                current_winner = (competidores[i]).join_params();
                ganados++;
                break;
            }
        }
        jugados ++;
    } while (centro != current_winner && jugados < 100);
    cerr << jugados << endl;
    return golosa(current_winner, this->columnas, this->filas, this->c);
}

golosa grid_search::random_busqueda_local_first_lose() {
    pesos centro = this->get_random_params();
    pesos anterior = centro;
    pesos current_winner = centro;
    // cout << "RANDOafdsfsdM" << endl;
    // for (uint i=0; i<current_winner.size();i++){
    //     cout << current_winner[i] << '\t';
    // }
    // cout << endl;

    int jugados = 0;
    int empates = 0;
    int ganados = 0;
    do {
        anterior = centro;
        centro = current_winner;
        vector<golosa> competidores = this->get_neighbors_golosos_v(centro);

        bool hubo_ganador = false;
        golosa g_centro = golosa(centro, this->columnas, this->filas, this->c);
        for (uint i = 0; i< competidores.size(); i++) {
            int result = ida_y_vuelta(this->columnas, this->filas, this->c, this->p, g_centro, competidores[i]);
            if (result == SEGUNDO){
                current_winner = (competidores[i]).join_params();
                ganados++;
                hubo_ganador = true;
                break;
            }
        }
        if (!hubo_ganador){
            for (uint i = 0; i< competidores.size(); i++) {
                int result = ida_y_vuelta(this->columnas, this->filas, this->c, this->p, g_centro, competidores[i]);
                if (result == EMPATE && (competidores[i]).join_params() != anterior){
                    current_winner = (competidores[i]).join_params();
                    if (result == SEGUNDO) ganados++;
                    empates++;
                    break;
                }
            }
        }
        // cout << "current_winner" << endl;
        // for (uint i=0; i<current_winner.size();i++){
        //     cout << current_winner[i] << '\t';
        // }
        // cout << endl;
        jugados ++;
    } while (centro != current_winner && empates < 15 && jugados < 100);

    //Cuando se repite 2 veces el mismo campeon salimos
    // for (uint i=0; i<current_winner.size();i++){
    //     cout << current_winner[i] << '\t';
    // }
    // cout << endl;
    // cout << "TRAS JUGAR " << jugados << " PARTIDOS" << endl;
    // cout << ganados << endl;
    cerr << ganados << endl;
    return golosa(current_winner, this->columnas, this->filas, this->c);
}

void grid_search::randomized_train() {
    golosa partial_best = this->random_busqueda_local_first_lose();
    int championships = 0;
    int total_matches = 0;
    golosa current_goloso = partial_best;
    while(championships < 100 && total_matches < 6000) {
        total_matches++;
        current_goloso = this->random_busqueda_local_first_lose();
        int result = ida_y_vuelta(this->columnas, this->filas, this->c, this->p, partial_best, current_goloso);
        if (result == SEGUNDO) {
            partial_best = current_goloso;
            cout << championships << endl;
            championships = 0;
        } else {
            championships ++;
        }
    }
    cout << "championships " << championships << ", total_matches " << total_matches << endl;

    pesos params = partial_best.join_params();
    for (uint i=0; i<params.size();i++){
        cout << params[i] << '\t';
    }
    cout << endl << endl;

    // ESTO ES LO DEL FIXTURE
    // list<golosa> triunfadores;
    // // for (int i=0; i< 50; i++) {
    //     triunfadores.push_back(this->random_busqueda_local_con_fixture());
    // // }
    // list<golosa> results = fixture_golosas(this->columnas, this->filas, this->c, this->p, triunfadores);
    // cout << "GOLOSO" << endl;
    // for (list<golosa>::iterator it = results.begin(); it != results.end(); ++it) {
    //     pesos params = (*it).join_params();
    //     for (uint i=0; i<params.size();i++){
    //         cout << params[i] << '\t';
    //     }
    //     cout << endl << endl;
    // }
}
