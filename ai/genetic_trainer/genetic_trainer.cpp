#include "genetic_trainer.hpp"
#include "../fitness.hpp"
#include <string.h>
#include <cstdlib>
#include <stdlib.h>  
#include <string>
#include <limits>

// los params se van a mover entre -CLIP_ABS_LIMIT y CLIP_ABS_LIMIT
#define CLIP_ABS_LIMIT 1000
// si el parametro es |parametro| < ZERO_CLIP, se clipea a 0
#define ZERO_CLIP 1e-20


#define EQUALS_FLOAT(a,b) ((-1e-4 <= a - b) && (a - b <= 1e-4))

pesos gen_trainer::train_random_fitness(){
    // asser que las poblaciones sean de tamaño mayor a 2
    assert(pop_size >= 2);
    // generate initial population
    vector< pesos > pop = vector< pesos >(this->pop_size);
    vector< pesos > new_pop;
    vector< float > pop_fitness(this->pop_size, -1);
    uint gen_count = 0;
    for (uint i = 0; i < pop_size; i++) {
        pop[i] = this->randon_genome();
    }

    // primer jugador golosa, va a ser uno random entre los genomas

    // empieza ciclo evolutivo
    do{
        gen_count++;
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "> generacion " << gen_count << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
        new_pop = vector< pesos >(pop_size);

        // selecciono segun su fitness lo progenitores

        std::cout << "> seleccionando progenitores" << std::endl;

        pesos p1 = this->random_selection(pop, pop_fitness);
        float p1_fitness = this->fitness_against_random(p1);
        std::cerr << gen_count << "," << 
            p1_fitness << std::endl;
        pesos p2 = this->random_selection(pop, pop_fitness);
        float p2_fitness = this->fitness_against_random(p2);

        std::cout << "> p1: " << p1_fitness << " ; p2: " << p2_fitness << std::endl;
        std::cout << "> genereando nueva población" << std::endl;

        for (uint i = 0; i < pop_size; i++) {
            // nace hijo
            pesos child = this->crossover(p1, p2);
            // muta
            this->mutate(child);
            // es wolverine
            new_pop[i] = child;
        }
        pop = new_pop;
        pop_fitness = vector<float>(pop.size(),-1);
    } while(gen_count < this->gen_limit);
    // pop_fitness y pop tiene las povlaciones y los correspondientes
    // valores de fitness de la última generación de individuos
    std::cout << "la evolucion a terminado!" << std::endl;

    pesos max_pesos; float max_fitness = -1;
    for (uint i = 0; i < pop.size(); i++) {
        if (EQUALS_FLOAT(pop_fitness[i], -1)) {
            pop_fitness[i] = this->fitness_against_random(pop[i]);
        }
        if (pop_fitness[i] > max_fitness) {
            max_fitness = pop_fitness[i];
            max_pesos = pop[i];
        }
    }
    std::cout << "Mayor fitness encontrado: " << max_fitness << std::endl;
    return this->clip_float_values(max_pesos);
}

/*
    ----------------------------
    Search via Genetic Algorithm
    ----------------------------
    population = generate initial population (random)
    do:
        new_population = {}
        for i=1 to population.size():
            x = random-selection(population) sorted by fitness function
            y = random-selection(population) sorted by fitness function
            child = crossover(x,y)
            if (p_mutation):
                mutate child
            add child to new_population
        population = new_population
    while !individual fit enough || generation limit
    return best individual from population
*/

pesos gen_trainer::train(uint pop_size){
    // asser que las poblaciones sean de tamaño mayor a 2
    assert(pop_size >= 2);
    // generate initial population
    vector< pesos > pop = vector< pesos >(pop_size);
    vector< pesos > new_pop;
    uint gen_count = 0;
    for (uint i = 0; i < pop_size; i++) {
        pop[i] = this->randon_genome();
    }

    // primer jugador golosa, va a ser uno random entre los genomas

    // empieza ciclo evolutivo
    do{
        gen_count++;
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "> generacion " << gen_count << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
        new_pop = vector< pesos >(pop_size);
        // genero golosas a partir de poblacion
        std::cout << "> genereando golosas" << std::endl;
        list<golosa> pob_golosas = this->golosas_from_pop(pop);
        // hago que todos jueguen el fixture
        std::cout << "> corriendo fixture" << std::endl;
        list<golosa> pob_rankings = 
            fixture_golosas(this->n, this->m, this->c, this->p, pob_golosas);
        // agarro dos padres
        // estos serán los correspondientes a las dos golosas mejor rakeadas
        auto it_rankings = pob_rankings.begin();
        pesos p1 = (*it_rankings).join_params();
        // printeando fitness padre 1
        std::cerr << gen_count << "," << 
            threaded_regular_fitness(this->n, this->m, this->c, this->p, p1,20000) << std::endl;

        it_rankings++;
        pesos p2 = (*it_rankings).join_params();
        std::cout << "> genereando nueva población" << std::endl;
        for (uint i = 0; i < pop_size; i++) {
            // nace hijo
            pesos child = this->crossover(p1, p2);
            // muta
            this->mutate(child);
            // es wolverine
            new_pop[i] = child;
        }
        pop = new_pop;
    } while(gen_count < this->gen_limit);
    // pop_fitness y pop tiene las povlaciones y los correspondientes
    // valores de fitness de la última generación de individuos
    std::cout << "la evolucion a terminado!" << std::endl;
    // genero golosas a partir de poblacion
    list<golosa> pob_golosas = this->golosas_from_pop(pop);
    // hago que todos jueguen el fixture
    list<golosa> pob_rankings = 
        fixture_golosas(this->n, this->m, this->c, this->p, pob_golosas);
    // agarro dos padres
    // estos serán los correspondientes a las dos golosas mejor rakeadas
    auto it_rankings = pob_rankings.begin();
    pesos max_pesos = (*it_rankings).join_params();
    return this->clip_float_values(max_pesos);
}

list<golosa> gen_trainer::golosas_from_pop( vector<pesos> ps ){
    list<golosa> l;
    for (vector<pesos>::const_iterator it = ps.begin();
           it != ps.end(); it++) {
        l.push_front(golosa(*it, this->n, this->m, this->c));
    }
    return l;
}

pesos gen_trainer::crossover(pesos p1, pesos p2){
    uint cross_section = rand() % this->param_count;
    if (cross_section == this->param_count - 1) {
        return p1;
    }else if (cross_section == 0) {
        return p2;
    }else{
        pesos p_son = pesos(this->param_count);
        for (uint i = 0; i < this->param_count; i++) {
            // copio los primeros cross_section parametros de p1, el resto de 2
            p_son[i] = i < cross_section ?  p1[i] : p2[i];
        }
        assert(-1 <= p_son[PRIMERA_JUGADA] && p_son[PRIMERA_JUGADA] <= this->n-1);
        return p_son;
    }
}

#define TOLERATION_LIMIT 0.01
void gen_trainer::mutate(pesos &p){
    float lottery = float(rand())/float(RAND_MAX); // lottery ~ U[0,1]
    if (lottery < this->p_mutation) {
        // mutation achieved
        uint mutation_idx = rand() % this->param_count;
        std::cout << "> hubo mutación en el gen " << mutation_idx << std::endl;
        if (mutation_idx != PRIMERA_JUGADA) {
            p[mutation_idx] = (int)this->__get_rand_float() % this->n;
        }else{ // mutation_idx == PRIMERA_JUGADA
            uint r = -1 + rand()%(this->n+1);
            p[mutation_idx] = float(r);
            std::cout << "Muto el gen 0: " << p[mutation_idx] << std::endl;
        }
    }
    //assert(-1 - TOLERATION_LIMIT <= p[PRIMERA_JUGADA] &&
            //p[PRIMERA_JUGADA] <= this->n-1 + TOLERATION_LIMIT);
}

pesos gen_trainer::random_selection(vector< pesos > ps, vector<float> &fs){
    // ps: poblacion de pesos actual
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



float gen_trainer::fitness_against_random(pesos p){
    return threaded_regular_fitness(this->n, this->m, this->c, this->p, p,10000);
}

pesos gen_trainer::randon_genome(){
    pesos p = pesos(this->param_count);
    for (uint i = 0; i < this->param_count; i++) {
        if (i == PRIMERA_JUGADA) {
            // estoy en PRIMERA_JUGADA
            int r = -1 + rand()%(this->n+1);
            p[PRIMERA_JUGADA] = float(r);
        }else{
            p[i] = this->__get_rand_float();
        }
    }
    assert(-1 <= p[PRIMERA_JUGADA] && p[0] <= this->n-1);
    return p;
}

// ADDED: Clipping
float gen_trainer::__get_rand_float(){
        return -CLIP_ABS_LIMIT + (static_cast <float> (rand()) / static_cast <float>(RAND_MAX))*
            (CLIP_ABS_LIMIT*2);
}

string gen_trainer::__to_argv(pesos p){
    string s;
    for (uint i = 0; i < p.size(); i++) {
        if (i == PRIMERA_JUGADA) {
            int t = (int)p[i];
            s += std::to_string(t);
        }else{
            s += std::to_string(p[i]);
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
