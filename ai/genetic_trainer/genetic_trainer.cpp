#include "genetic_trainer.hpp"
#include "../fitness.hpp"
#include <string.h>
#include <cstdlib>
#include <stdlib.h>  
#include <string>

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
    vector<float> pop_fitness;
    vector< pesos > new_pop;
    uint gen_count = 0;
    for (uint i = 0; i < pop_size; i++) {
        pop[i] = this->randon_genome();
    }

    // primer jugador golosa, va a ser uno random entre los genomas

    // empieza ciclo evolutivo
    do{
        gen_count++;
        std::cout << "-------------------------------------------------------" << gen_count << std::endl;
        std::cout << "> generacion " << gen_count << std::endl;
        std::cout << "-------------------------------------------------------" << gen_count << std::endl;
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
    this->max_achieved = max_pesos;
    return max_pesos;
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
        return p_son;
    }
}


golosa gen_trainer::crossover(golosa g1, golosa g2){
    uint cross_section = rand() % this->param_count; // 0 <= cross_section <= this->param_count - 1
    if (cross_section == this->param_count - 1) {
        // g1 queda en el crossover
        return g1;
    }else if(cross_section == 0){
        // g2 queda en el crossover
        return g2;
    }else{ // 0 < cross_section < this->param_count - 1 
        vector<float> cross_p(this->param_count); 
        for (uint i = 0; i < this->param_count; i++) {
            if (i < PARAM_COUNT) {
                // copiando de parametros
                cross_p[i] = i < cross_section ?  g1.ver_parametros()[i] : g1.ver_parametros()[i];
            }else{
                // copiando de pesos lineales
                cross_p[i] = i < cross_section ?  g1.ver_pesos_lineas()[i] : g2.ver_pesos_lineas()[i];
            }
        }
        return golosa(cross_p, this->n, this->m, this->c);
    }
}


void gen_trainer::mutate(pesos &p){
    float lottery = rand()/RAND_MAX; // lottery ~ U[0,1]
    if (lottery < this->p_mutation) {
        // mutation achieved
        uint mutation_idx = rand() % this->param_count;
        std::cout << "> hubo mutación en el gen " << mutation_idx << std::endl;
        if (mutation_idx != PRIMERA_JUGADA) {
            p[mutation_idx] = (int)this->__get_rand_float() % this->n;
        }else{
            p[mutation_idx] = this->__get_rand_float();
        }
    }
}

pesos gen_trainer::random_selection(vector< pesos > ps, vector<float> &fs){
    // ps: poblacion de pesos actual
    // fs: fitness converetido en proba fs(p) = 1 - (1/fitness(p))
    // proba entre 0 y 1, cuanto mayor el fitness, mayor la proba
    // Si fs tiene -1, es que no fue calculado rodavía
    while(true){
        uint chosen_index = rand() % ps.size();
        float lottery_num = rand() / RAND_MAX;
        if (fs[chosen_index] == -1) {
            // todavia no se calculo el fintess de ese genoma
            fs[chosen_index] = this->fitness(ps[chosen_index]);
            std::cout << "Se calculo el fitness de " << chosen_index << " y dio " << fs[chosen_index] << std::endl;
        
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


float gen_trainer::fitness(pesos p){
    return regular_fitness(this->n, this->m, this->c, 100000, p);
}

pesos gen_trainer::get_max() const{
    return this->max_achieved;
}

pesos gen_trainer::randon_genome(){
    pesos p = pesos(this->param_count);
    for (uint i = 0; i < this->param_count; i++) {
        if (i == PRIMERA_JUGADA) {
            int rand_int = rand() * ((rand() < RAND_MAX/2) ? -1 : 1);
            // primera jugada debe ser entre -1 y N-1
            p[i] = rand_int < 0 ? -1 : rand_int % this->n;
        }else{
            p[i] = this->__get_rand_float();
        }
    }
    return p;
}

float gen_trainer::__get_rand_float(){
        int random_num = rand();
        // random_num &= 0x7FFFFFFF; // bit de signo en positivo
        void* ptr = &random_num;
        return *((float*)ptr);
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
