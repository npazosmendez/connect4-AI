#include "genetic_trainer.hpp"
#include "../fitness.hpp"
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <limits>
#include <fstream>

using namespace std;

#define CLIP_ABS_LIMIT 100
#define CANT_PARTIDOS_RANDOM 20000
#define CANT_PARTIDOS_GOLOSOS 1000

// Logging
ofstream best_fitness_log; // log para el mejor fitness de cada generación
ofstream progenitores_log; // log para los 2 progenitores de cada generación
ofstream champion_log; // log para los parámetros finales encontrados
void open_log(){
    if (system("mkdir -p log") || system("rm -f log/best_fitness_log.dat log/best_fitness_log.dat log/ganador.dat")) {
        cerr << "Error iniciando logs." << endl;
        exit(1);
    }
    best_fitness_log.open("log/best_fitness_log.dat");
    progenitores_log.open("log/progenitores.dat");
    champion_log.open("log/ganador.dat");
}

void close_log(){
    best_fitness_log.close();
    progenitores_log.close();
    champion_log.close();
}


#define CONSOLE_LOG_LEVEL 2
class mystreambuf: public streambuf{};
mystreambuf nostreambuf;
std::ostream nocout(&nostreambuf); // ostream que no se ve
#define log(x) ((x <= CONSOLE_LOG_LEVEL) ? cout : nocout)


////////////////////////////////////
/* FUNCIÓN PRINCIPAL DE TRAINING */
////////////////////////////////////
vector<float> gen_trainer::train(){
    // assert que las poblaciones sean de tamaño mayor a 2
    assert(pop_size >= 2);
    open_log();

    log(1) << "-------------------------------------------------------" << endl;
    log(1) << "Población: \t" << pop_size << endl;
    log(1) << "Generaciones: \t" << gen_limit << endl;
    log(1) << "P(mutación): \t" << p_mutation << endl;
    log(1) << "Fitness: \t" << ((FITNESS==VS_RANDOM) ? "vs random" : ((FITNESS==FIXTURE) ? "fixture" : "vs golosos random")) << endl;
    log(1) << "Crossover: \t" << ((CROSSOVER==COMPLETO) ? "completo" : "binario") << endl;
    log(1) << "Selección: \t" << ((SELECCION==RANK) ? "por rank" : "probabilística") << endl;
    log(1) << "Mutación: \t" << ((MUTATION==MONO) ? "mono" : "multi") << endl;
    log(1) << "-------------------------------------------------------" << endl;

    uint generacion = 0;
    vector< float > pop_fitness(pop_size); // fitness de cada chabón
    vector< vector<float> > pop(pop_size); // población
    vector<float> p1,p2; // progenitores
    vector<float> alfa(param_count); // el mejor encontrado hasta ahora
    float global_max_fitness = -1;

    // Población inicial, aleatoria
    for (uint i = 0; i < pop_size; i++)
    pop[i] = randon_genome();

    // Itero: fitness, crossover, mutación.
    do{
        generacion++;
        log(1) << "-------------------------------------------------------" << endl;
        log(1) << "> generacion " << generacion << endl;
        log(1) << "-------------------------------------------------------" << endl;

        ///////////////////////
        // Calculos los fitness
        ///////////////////////
        log(2) << "> calculando fitness" << endl;
        switch (FITNESS) {
            case VS_RANDOM:
            for (size_t i = 0; i < pop_size; i++) pop_fitness[i] = fitness_against_random(pop[i]);
            break;
            case FIXTURE:
            pop_fitness = fitness_fixture(pop);
            break;
            case VS_GOLOSOS:
            for (size_t i = 0; i < pop_size; i++) pop_fitness[i] = fitness_against_golosos(pop[i]);
            break;
        }
        float max = -1;
        int k = -1;
        for (uint i = 0; i < pop_fitness.size(); i++) {
            if (pop_fitness[i] > max) {
                k = i;
                max = pop_fitness[i];
            }
        }
        if (max > global_max_fitness){
            alfa = pop[k];
            global_max_fitness = max;
        }
        log(2) << "\tmejor fitness de la generación: " << max << endl;
        best_fitness_log << max << endl; // Imprimio el mejor fitness


        /////////////////////////////////////////
        // Selecciono dos progenitores, los capos
        /////////////////////////////////////////
        log(2) << "> seleccionando 2 progenitores" << endl;
        switch (SELECCION) {
            case PROBABILISTICA:
                progenitores_probabilisticos(p1,p2,pop_fitness, pop);
                break;
            case RANK:
                progenitores_por_fitness(p1,p2,pop_fitness, pop);
                break;
        }
        // Consola
        for (size_t i = 0; i < p1.size(); i++) log(2) << p1[i] << "\t";
        log(2) << endl;
        for (size_t i = 0; i < p2.size(); i++) log(2) << p2[i] << "\t";
        log(2) << endl;
        // Log
        progenitores_log << p1[0];
        for (size_t i = 1; i < p1.size(); i++) progenitores_log <<","<< p1[i];
        progenitores_log << endl;
        progenitores_log << p2[0];
        for (size_t i = 1; i < p2.size(); i++) progenitores_log <<","<< p2[i];
        progenitores_log << endl;



        ///////////////////////////////////////////////
        // Crossover y mutación, genero nueva población
        ///////////////////////////////////////////////
        log(2) << "> genereando nueva población" << endl;
        pop[0] = p1; // me aseguro de quedarme con los buenos completos
        pop[1] = p2;
        for (uint i = 2; i < pop_size; i++) {
            // nace hijo
            vector<float> child;
            switch (CROSSOVER) {
                case COMPLETO:
                child = this->crossover_completo(p1, p2);
                break;
                case BINARIO:
                child = this->crossover_binario(p1, p2);
                break;
            }
            switch (MUTATION) {
                case MONO:
                    mono_mutate(child);
                    break;
                case MULTI:
                    multi_mutate(child);
                    break;
            }
            pop[i] = child;
        }
    } while(generacion < gen_limit);


    ////////////////////
    // FIN DE EVOLUCIÓN
    ////////////////////
    log(1) << "-------------------------------------------------------" << endl;
    log(1) << "la evolucion a terminado!" << endl;
    log(1) << "Mayor fitness encontrado: " << global_max_fitness << endl;

    champion_log << global_max_fitness << endl;
    champion_log << alfa[0];
    for (size_t i = 1; i < alfa.size(); i++) champion_log <<","<< alfa[i];
    champion_log << endl;

    close_log();
    return (alfa);
}


/////////////////////////////
/////////* FITNESS */////////
/////////////////////////////

vector<float> gen_trainer::fitness_fixture(const vector< vector<float> > &pop){
    /* Fixture tipo fútbol, todos contra todos ida y vuelta. */
    /* El fixture es entre los sujetos de una generación, y
    el fitness corresponde a la proporción de ganados/jugados */
    vector<float> wins(pop.size(),0);
    float total_wins = 0;
    for (size_t i = 0; i < pop.size(); i++) {
        for (size_t j = 0; j < pop.size(); j++) {
            if (i == j) continue;
            golosa ai1(pop[i], n, m, c);
            golosa ai2(pop[j], n, m, c);
            uint match = golosa_vs_golosa(n,m,c,p,ai1,ai2);
            switch (match) {
                case PRIMERO:
                wins[i] += 1;
                total_wins += 1;
                break;
                case SEGUNDO:
                wins[j] += 1;
                total_wins += 1;
                break;
            }
        }
    }
    for (size_t i = 0; i < wins.size(); i++) wins[i] /= (2*pop.size()-2);
    return wins;
}

float gen_trainer::fitness_against_random(vector<float> p){
    /* Calcula el fitnes de un sujeto como ganados/jugados
    contra un jugador random. Juega tantas veces como indique
    CANT_PARTIDOS_RANDOM. */
    return threaded_regular_fitness(this->n, this->m, this->c, this->p, p,CANT_PARTIDOS_RANDOM);
}
float gen_trainer::fitness_against_golosos(vector<float> &chabon){
    /* Misma idea que jugar contra random muchas veces, pero en vez
    de jugar contra random, se juega contra golosos de parámetros
    random. Creería que es un poco mejor que el fixture, porque el
    análisis no está tan cegado a una generación.
    El fitness corresponde a ganados/jugados, y se juegan tantos
    como CANT_PARTIDOS_GOLOSOS. */
    uint ganados = 0;
    golosa ai(chabon, n, m, c);
    for (size_t i = 0; i < CANT_PARTIDOS_GOLOSOS/2; i++) {
        golosa random(gen_trainer::randon_genome(), n, m, c);
        uint match = golosa_vs_golosa(n,m,c,p,ai,random);
        if (match == PRIMERO){
            ganados++;
        }
        match = golosa_vs_golosa(n,m,c,p,random,ai);
        if (match == SEGUNDO){
            ganados++;
        }
    }
    return (float)ganados/CANT_PARTIDOS_GOLOSOS;
}


/////////////////////////////
///////* CROSSOVER */////////
/////////////////////////////

vector<float> gen_trainer::crossover_completo(vector<float> p1, vector<float> p2){
    /*  La partición del crossover es lo más granular posible: gen por gen */
    vector<float> hijo(param_count);
    for (size_t i = 0; i < param_count; i++) {
        uint coin = rand() % 2;
        if (coin == 1) {
            hijo[i] = p1[i];
        }else{
            hijo[i] = p2[i];
        }
    }
    return hijo;
}

vector<float> gen_trainer::crossover_binario(vector<float> p1, vector<float> p2){
    /* El crossover se hace con una sola partición */
    uint cross_section = rand() % param_count;
    if (cross_section == this->param_count - 1) {
        return p1;
    }else if (cross_section == 0) {
        return p2;
    }else{
        vector<float> p_son(param_count);
        for (uint i = 0; i < this->param_count; i++) {
            // copio los primeros cross_section parametros de p1, el resto de 2
            p_son[i] = i < cross_section ?  p1[i] : p2[i];
        }
        assert(-1 <= p_son[PRIMERA_JUGADA] && p_son[PRIMERA_JUGADA] <= this->n-1);
        cout << "Hijo: ";
        for (size_t i = 0; i < p_son.size(); i++) cout << p_son[i] << " ";
        cout << endl;
        return p_son;
    }
}


/////////////////////////////
////////* MTUATION */////////
/////////////////////////////

bool gen_trainer::mono_mutate(vector<float> &p){
    /* Si se muta, se lo hace en un único gen aleatorio */
    float lottery = float(rand())/float(RAND_MAX); // lottery ~ U[0,1]
    if (lottery < p_mutation) {
        // mutation achieved
        uint mutation_idx = rand() % param_count;
        log(3) << "> hubo mutación en el gen " << mutation_idx << endl;
        if (mutation_idx != PRIMERA_JUGADA) {
            p[mutation_idx] = -CLIP_ABS_LIMIT + rand()%(CLIP_ABS_LIMIT*2);
            log(3) << "Muto el gen " << mutation_idx <<": "<< p[mutation_idx] << endl;
        }else{ // mutation_idx == PRIMERA_JUGADA
            int r = -1 + rand()%(n+1);
            p[mutation_idx] = r;
            log(3) << "Muto el gen 0: " << p[mutation_idx] << endl;
        }
        return true;
    }
    return false;

}
bool gen_trainer::multi_mutate(vector<float> &p){
    /* En cada gen se tira una moneda para ver si se muta o no. Es decir,
    podrían llegar a mutarse todos los genes de un único individuo */
    bool res = false;
    for (size_t i = 0; i < param_count; i++) {
        float lottery = float(rand())/float(RAND_MAX); // lottery ~ U[0,1]
        if (lottery < p_mutation) {
            log(3) << "> hubo mutación en el gen " << i << endl;
            if (i != PRIMERA_JUGADA) {
                p[i] = -CLIP_ABS_LIMIT + rand()%(CLIP_ABS_LIMIT*2);
                p[i] *= max(rand()%2,rand()%2); // P(x=0) = 0.25
                log(3) << "Muto el gen " << i <<": "<< p[i] << endl;
            }else{ // i == PRIMERA_JUGADA
                int r = -1 + rand()%(n+1);
                p[i] = r;
                log(3) << "Muto el gen 0: " << p[i] << endl;
            }
            res = true;
        }
    }
    return res;
}

vector<float> gen_trainer::randon_genome(){
    /* Crea un genoma aleatorio para comenzar */
    vector<float> p = vector<float>(param_count);
    for (uint i = 0; i < param_count; i++) {
        if (i == PRIMERA_JUGADA) {
            // estoy en PRIMERA_JUGADA
            int r = -1 + rand()%(n+1);
            p[PRIMERA_JUGADA] = r;
        }else{
            p[i] = -CLIP_ABS_LIMIT + rand()%(CLIP_ABS_LIMIT*2);
        }
    }
    assert(-1 <= p[PRIMERA_JUGADA] && p[0] <= n-1);
    return p;
}

////////////////////////////
////////* SELECCIÓN *///////
////////////////////////////


void gen_trainer::progenitores_probabilisticos(vector<float> &p1, vector<float> &p2, const vector<float> &pop_fitness, const vector< vector<float> > &pop){
    /* Selecciona un chabón con proba proporcional a su fitness */

    // Normalizo los fitness para que sumen 1
    vector<float> distribucion(pop.size());
    float suma_total = 0;
    for (size_t i = 0; i < pop_fitness.size(); i++) suma_total += pop_fitness[i];
    for (size_t i = 0; i < distribucion.size(); i++) distribucion[i] = pop_fitness[i]/suma_total;
    // Tomo dos muestras U[0,1]
    float muestra1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float muestra2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    bool elegi1 = false;
    bool elegi2 = false;
    float acum = 0;
    // Me fijo donde cayeron las 2 muestras
    for (uint i = 0; i < distribucion.size(); i++) {
        acum += distribucion[i];
        if (acum >= muestra1 && !elegi1){
            elegi1 = true;
            p1 = pop[i];
        }
        if (acum >= muestra2 && !elegi2){
            elegi2 = true;
            p2 = pop[i];
        }
    }
}
void gen_trainer::progenitores_por_fitness(vector<float> &p1, vector<float> &p2, const vector<float> &pop_fitness, const vector< vector<float> > &pop){
    /* Selecciona los 2 chabones con más fitness */
    float max = -1;
    uint max_idx1 = 0;
    for (uint i = 0; i < pop_fitness.size(); i++) {
        if (max < pop_fitness[i]) {
            max = pop_fitness[i];
            max_idx1 = i;
        }
    }
    p1 = pop[max_idx1];
    max = -1;
    uint max_idx2 = 0;
    for (uint i = 0; i < pop_fitness.size(); i++) {
        if (max < pop_fitness[i] && i != max_idx1) {
            max = pop_fitness[i];
            max_idx2 = i;
        }
    }
    p2 = pop[max_idx2];
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*

vector<float> gen_trainer::random_selection(vector< vector<float> > ps, vector<float> &fs){
    // ps: poblacion de vector<float> actual
    // fs: fitness converetido en proba fs(p) = 1 - (1/fitness(p))
    // proba entre 0 y 1, cuanto mayor el fitness, mayor la proba
    // Si fs tiene -1, es que no fue calculado rodavía
    while(true){
        uint chosen_index = rand() % ps.size();
        float lottery_num = rand() / RAND_MAX;
        if (EQUALS_FLOAT(fs[chosen_index],-1)) {
            // todavia no se calculo el fintess de ese genoma
            fs[chosen_index] = this->fitness_against_random(ps[chosen_index]);
        }
        if (lottery_num < fs[chosen_index]) {
            // si el float en [0,1] sorteado es menor a la proba
            // que sale de evaluar el fitness, de forma que si es alta,
            // es más probableq que quede en ese intervalo, se elige ese
            // peso
            return ps[chosen_index];
        }
    }
}


// ADDED: Clipping
float gen_trainer::__get_rand_float(){
    return -CLIP_ABS_LIMIT + (static_cast <float> (rand()) / static_cast <float>(RAND_MAX))*
    (CLIP_ABS_LIMIT*2);
}

string gen_trainer::__to_argv(vector<float> p){
    string s;
    for (uint i = 0; i < p.size(); i++) {
        if (i == PRIMERA_JUGADA) {
            int t = (int)p[i];
            s += to_string(t);
        }else{
            s += to_string(p[i]);
        }
    }
    return s;
}

vector<float> gen_trainer::clip_float_values(vector<float> v){
    for (uint i = 0; i < v.size(); i++) {
        if (i != PRIMERA_JUGADA)
        if (-ZERO_CLIP <= v[i] && v[i] <= ZERO_CLIP)
        v[i] = 0;
    }
    return v;
}

list<golosa> gen_trainer::golosas_from_pop( vector<vector<float>> ps ){
    list<golosa> l;
    for (vector<vector<float>>::const_iterator it = ps.begin();
    it != ps.end(); it++) {
        l.push_front(golosa(*it, this->n, this->m, this->c));
    }
    return l;
}


vector<float> gen_trainer::train(uint pop_size){
    // asser que las poblaciones sean de tamaño mayor a 2
    assert(pop_size >= 2);
    // generate initial population
    vector< vector<float> > pop = vector< vector<float> >(pop_size);
    vector< vector<float> > new_pop;
    uint gen_count = 0;
    for (uint i = 0; i < pop_size; i++) {
        pop[i] = this->randon_genome();
    }

    // primer jugador golosa, va a ser uno random entre los genomas

    // empieza ciclo evolutivo
    do{
        gen_count++;
        cout << "-------------------------------------------------------" << endl;
        cout << "> generacion " << gen_count << endl;
        cout << "-------------------------------------------------------" << endl;
        new_pop = vector< vector<float> >(pop_size);
        // genero golosas a partir de poblacion
        cout << "> genereando golosas" << endl;
        list<golosa> pob_golosas = this->golosas_from_pop(pop);
        // hago que todos jueguen el fixture
        cout << "> corriendo fixture" << endl;
        list<golosa> pob_rankings =
        fixture_golosas(this->n, this->m, this->c, this->p, pob_golosas);
        // agarro dos padres
        // estos serán los correspondientes a las dos golosas mejor rakeadas
        auto it_rankings = pob_rankings.begin();
        vector<float> p1 = (*it_rankings).join_params();
        // printeando fitness padre 1
        cerr << gen_count << "," <<
        threaded_regular_fitness(this->n, this->m, this->c, this->p, p1,20000) << endl;

        it_rankings++;
        vector<float> p2 = (*it_rankings).join_params();
        cout << "> genereando nueva población" << endl;
        for (uint i = 0; i < pop_size; i++) {
            // nace hijo
            vector<float> child = this->crossover(p1, p2);
            // muta
            this->mono_mutate(child);
            // es wolverine
            new_pop[i] = child;
        }
        pop = new_pop;
    } while(gen_count < this->gen_limit);
    // pop_fitness y pop tiene las povlaciones y los correspondientes
    // valores de fitness de la última generación de individuos
    cout << "la evolucion a terminado!" << endl;
    // genero golosas a partir de poblacion
    list<golosa> pob_golosas = this->golosas_from_pop(pop);
    // hago que todos jueguen el fixture
    list<golosa> pob_rankings =
    fixture_golosas(this->n, this->m, this->c, this->p, pob_golosas);
    // agarro dos padres
    // estos serán los correspondientes a las dos golosas mejor rakeadas
    auto it_rankings = pob_rankings.begin();
    vector<float> max_pesos = (*it_rankings).join_params();
    return this->clip_float_values(max_pesos);
}

*/
