#include "genetic_trainer.hpp"
#include <string.h>
#include <cstdlib>


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
    // generate initial population
    vector<pesos> pop = vector<pesos>(pop_size);
    vector<float> pop_fitness;
    uint gen_count = 0;
    for (uint i = 0; i < pop_size; i++) {
        pop[i] = this->randon_genome();
    }
    // empieza ciclo evolutivo
    do{
        gen_count++;
        vector<pesos> new_pop = vector<pesos>(pop_size);
        pop_fitness = vector<float>(pop_size);
        for (uint i = 0; i < pop_size; i++) {
            // agarro dos padres
            pesos p1 = this->random_selection(pop, pop_fitness);
            pesos p2 = this->random_selection(pop, pop_fitness);
            // nace hijo
            pesos child = this->crossover(p1, p2);
            // muta
            this->mutate(child);
            // es wolverine
            new_pop[i] = child;
        }
        pop = new_pop;
    }while(gen_count < this->gen_limit);
    // pop_fitness y pop tiene las povlaciones y los correspondientes
    // valores de fitness de la última generación de individuos
    pesos max_pesos; float max_fit = -1;
    for (uint i = 0; i < pop_size; i++) {
        if (pop_fitness[i] == -1) {
            pop_fitness[i] = this->fitness(pop[i]);
        }
        if (max_fit < pop_fitness[i]) {
            max_pesos = pop[i];
            max_fit = pop_fitness[i];
        }
    }
    return max_pesos;
}


pesos gen_trainer::crossover(pesos p1, pesos p2){
    uint cross_section = rand() % PESOS_COUNT + 1;
    pesos p_son = pesos();
    // inicio el vector de altura del hijo de p1 y p2
    p_son.alturas = vector<float> (p1.alturas.size());
    // casteo p1, p2 y p_son a float* para hacer el crossover mas facil
    float* son_values = (float*) &p_son;
    float* p1_values = (float*) &p1;
    float* p2_values = (float*) &p2;
    // copy first 'cross_section' float parameters
    memcpy((void*) son_values, (void*) p1_values, sizeof(float)*cross_section);
    // increment pointer in corresponding qty
    son_values+=cross_section;
    p2_values+=cross_section;
    if (cross_section < PESOS_COUNT) {
        // el crossover involucra parte del genonma de p2
        // PESOS_COUNT-cross_section >= 1
        memcpy((void*) son_values, (void*) p2_values, sizeof(float)*(PESOS_COUNT-cross_section));
    } // up to this point, p1 and p2 float parameteres have been crossed over
    // crossover array of heights
    uint heights_cross_section = rand() % p1.alturas.size();
    for (uint i = 0; i <= heights_cross_section; i++) {
        p_son.alturas[i] = p1.alturas[i];
    }
    if (heights_cross_section < p1.alturas.size() - 1) {
        for (uint i = heights_cross_section+1; i < p1.alturas.size(); i++) {
            p_son.alturas[i] = p1.alturas[i];
        }
    }
    return p_son;
}

void gen_trainer::mutate(pesos &p){
    float lottery = rand()/RAND_MAX; // lottery ~ U[0,1]
    if (lottery < this->p_mutation) {
        // mutation achieved
        uint mutation_idx = rand() % PESOS_COUNT;
        float* p_floats = (float*) &p;
        int random_num = rand();
        random_num &= 0x7FFFFFFF; // bit de signo en positivo
        // copy int bytes assuming is float, in order to get random number 
        memcpy((void*)&p_floats[mutation_idx], &random_num, sizeof(float));
        // randomizo algun valor del vector alturas tambien? o lo cuento en mutation_idx y
        // sale sorteado ahi?
    }
}

pesos gen_trainer::random_selection(vector<pesos> ps, vector<float> &fs){
    // ps: poblacion de pesos actual
    // fs: fitness converetido en proba fs(p) = 1 - (1/fitness(p))
    // proba entre 0 y 1, cuanto mayor el fitness, mayor la proba
    // Si fs tiene -1, es que no fue calculado rodavía
    while(true){
        uint chosen_index = rand() % ps.size();
        float lottery_num = rand() % RAND_MAX;
        if (fs[chosen_index] == -1) 
            // todavia no se calculo el fintess de ese genoma
            fs[chosen_index] = this->fitness(ps[chosen_index]);
        if (lottery_num < fs[chosen_index]) {
            // si el float en [0,1] sorteado es menor a la proba
            // que sale de evaluar el fitness, de forma que si es alta,
            // es más probableq que quede en ese intervalo, se elige ese 
            // peso 
            return ps[chosen_index];
        }
    }
}

uint gen_trainer::fitness(pesos p){
    return 0;
}

pesos gen_trainer::get_max() const{
    return this->max_achieved;
}

pesos gen_trainer::randon_genome(){
    pesos p;
    // randomizo los pesos de los feature no altura
    for (int i = 0; i < PESOS_COUNT; i++) {
        float* p_floats = (float*) &p;
        int random_num = rand();
        random_num &= 0x7FFFFFFF; // bit de signo en positivo
        // copy int bytes assuming is float, in order to get random number 
        memcpy((void*)&p_floats[i], &random_num, sizeof(float));
    }
    // asumo n era columnas
    p.alturas = vector<float>(this->n);
    //float* p_alturas_floats = (float*) p.alturas.data();
    // mismo de randomizar, pero con pesos alturas
    for (uint i = 0; i < this->n; i++) {
        p.alturas[i] = this->__get_rand_float();
        //int random_num = rand();
        //random_num &= 0x7FFFFFFF; // bit de signo en positivo
        //// copy int bytes assuming is float, in order to get random number 
        //memcpy((void*)&p_alturas_floats[i], &random_num, sizeof(float));
    }
    return p;
}

float gen_trainer::__get_rand_float(){
        int random_num = rand();
        random_num &= 0x7FFFFFFF; // bit de signo en positivo
        void* ptr = &random_num;
        return *((float*)ptr);
}
