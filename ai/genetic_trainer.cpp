#include "genetic_trainer.hpp"
#include <string.h>


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
pesos gen_trainer::train(){

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
    uint heights_cross_section = rand() % p1.alturas.size() + 1;
    // TODO: crossover alturas

}

pesos gen_trainer::mutate(pesos p){

}

pesos gen_trainer::random_selection(vector<pesos> ps){

}

uint gen_trainer::fitness(pesos p){

}



pesos gen_trainer::get_max() const{

}

