#include "genetic_trainer.hpp"


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

}

pesos gen_trainer::mutate(pesos p){

}

pesos gen_trainer::random_selection(vector<pesos> ps){

}

uint gen_trainer::fitness(pesos p){

}



pesos gen_trainer::get_max() const{

}

