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

void grid_search::thorough_train(){
    //Precondiciones, pueden cambiar
    pesos params((int) golosa::cuantos_parametros(this->columnas,this->filas,this->c), this->inf_limit);
    golosa en_pelea = golosa(params, this->columnas, this->filas, this->c);
    int cant_en_pelea = 0;
    golosa mas_victorias = golosa(params, this->columnas, this->filas, this->c);
    int cant_mas_victorias = 0;
    do{
        golosa current = golosa(params, this->columnas, this->filas, this->c);
        int result = ida_y_vuelta(this->columnas, this->filas, this->c, this->p, en_pelea, current);
        if (result == SEGUNDO){
            if (cant_en_pelea >= cant_mas_victorias){
                cout << "Nuevo max tras " << cant_en_pelea << " peleas" << endl;
                pesos peso = en_pelea.join_params();
                for (uint i=0; i<peso.size();i++){
                    cout << peso[i] << "\t ";
                }
                cout << endl << endl << endl;

                cant_mas_victorias = cant_en_pelea;
                mas_victorias = en_pelea;
            }
            en_pelea = current;
            cant_en_pelea = 0;
        } else {
            cant_en_pelea++;
        }
    } while (avanzar(params, this->inf_limit, this->sup_limit, this->step, 1, (uint)params.size()));

    if (cant_en_pelea >= cant_mas_victorias){
        cant_mas_victorias = cant_en_pelea;
        mas_victorias = en_pelea;
    }


    params = mas_victorias.join_params();
    cout << "MAX con peleas en podio " << cant_mas_victorias << endl;
    for (uint i=0; i<params.size();i++){
        cout << params[i] << "\t ";
    }
    cout << endl << endl << endl;
}


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
    // cerr << jugados << endl;
    return golosa(current_winner, this->columnas, this->filas, this->c);
}

golosa grid_search::random_busqueda_local_first_lose() {
    pesos centro = this->get_random_params();
    golosa primero = golosa(centro, this->columnas, this->filas, this->c);
    pesos anterior = centro;
    pesos current_winner = centro;
    // cerr << threaded_regular_fitness(this->columnas, this->filas, this->c, this->p, centro, 20000) << ',';

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
                    empates++;
                    break;
                }
            }
        }
        jugados ++;
    } while (centro != current_winner && empates < 15 && jugados < 100);
    //Cuando se repite 2 veces el mismo campeon salimos

    // cerr << threaded_regular_fitness(this->columnas, this->filas, this->c, this->p, centro, 20000) << ',';
    // cerr << ganados << ',';
    // golosa a (current_winner, this->columnas, this->filas, this->c);
    // cerr << ida_y_vuelta(this->columnas, this->filas, this->c, this->p, primero, a) << endl;
    return golosa(current_winner, this->columnas, this->filas, this->c);
}

void grid_search::randomized_train() {
    cout << "fitness, championships" << endl;
    golosa partial_best = this->random_busqueda_local_first_lose();
    int championships = 0;
    int total_matches = 0;
    golosa current_goloso = partial_best;
    while(championships < 50 && total_matches < 1000) {
        total_matches++;
        current_goloso = this->random_busqueda_local_first_lose();
        int result = ida_y_vuelta(this->columnas, this->filas, this->c, this->p, partial_best, current_goloso);
        if (result == SEGUNDO) {
            pesos pes = partial_best.join_params();
            cout << threaded_regular_fitness(this->columnas, this->filas, this->c, this->p, pes , 20000) << ", " << championships << endl;
            partial_best = current_goloso;
            championships = 0;
        } else {
            championships ++;
        }
    }
    pesos pes = partial_best.join_params();
    cout << threaded_regular_fitness(this->columnas, this->filas, this->c, this->p, pes , 20000) << ", " << championships << endl;
    // cout << "championships " << championships << ", total_matches " << total_matches << endl;
    //
    cout << endl << endl;
    pesos params = partial_best.join_params();
    for (uint i=0; i<params.size();i++){
        cout << params[i] << '\t';
    }
    cout << endl << endl;
}
