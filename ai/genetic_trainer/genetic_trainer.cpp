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
    // generate initial population
    vector< pesos > pop = vector< pesos >(pop_size);
    vector<float> pop_fitness;
    vector< pesos > new_pop;
    uint gen_count = 0;
    for (uint i = 0; i < pop_size; i++) {
        pop[i] = this->randon_genome();
    }
    // empieza ciclo evolutivo
    do{
        gen_count++;
        std::cout << "generacion " << gen_count << std::endl;
        new_pop = vector< pesos >(pop_size);
        pop_fitness = vector<float>(pop_size , -1);
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
    } while(gen_count < this->gen_limit);
    // pop_fitness y pop tiene las povlaciones y los correspondientes
    // valores de fitness de la última generación de individuos
    std::cout << "la evolucion a terminado!" << std::endl;
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
    this->max_achieved = max_pesos;
    this->max_fitness_achieved = max_fit;
    return max_pesos;
}


pesos gen_trainer::crossover(pesos p1, pesos p2){
    uint cross_section = rand() % this->param_count + 1;
    if (cross_section == this->param_count) {
        return p1;
    }else{
        pesos p_son = pesos(this->param_count);
        for (uint i = 0; i < this->param_count; i++) {
            // copio los primeros cross_section parametros de p1, el resto de 2
            p_son[i] = i < cross_section ?  p1[i] : p2[i];
        }
        return p_son;
    }
}

void gen_trainer::mutate(pesos &p){
    float lottery = rand()/RAND_MAX; // lottery ~ U[0,1]
    if (lottery < this->p_mutation) {
        std::cout << "mutando perro" << std::endl;
        // mutation achieved
        uint mutation_idx = rand() % this->param_count;
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


/*
   IDEA
Pongo a correr c_linea una cierta cantidad de iteraciones en las que este empieza,
y la misma cantidad en las que este va segundo. De parseo el log y veo cuantas gane.
 - Jugar X/2 veces contra el random siendo primero
 - Jugar X/2 veces contra el random siendo último
 - fitness = 1 - 1/wins
*/

//"python2 c_linea.py —blue_player ./random_player —first rojo —red_player ./random_player —iterations 50"
/*
uint gen_trainer::fitness(pesos p){
    // TODO: Pasarle parametros al jugador goloso
    uint iterations = 50; // total
    string command = "python ../c_linea.py --blue_player ./random_player --first ";
    // primero empieza rojo (yo)
    string call = string(command);
    call += "rojo --iterations " + std::to_string(iterations/2);
    call += " --red_player ./golosa ";
    call += this->__to_argv(p);
    std::system(call.c_str());
    float wins_1 = (float)contar_victorias("rojo");
    // segundo empieza azul (random)
    call = string(command);
    call += "azul --iterations " + std::to_string(iterations/2);
    call += std::to_string(iterations/2);
    call += " --red_player ./golosa ";
    call += this->__to_argv(p);
    std::system(call.c_str());
    float wins_2 = (float)contar_victorias("rojo");
    return (wins_1 + wins_2) / iterations;
}
*/

float gen_trainer::fitness(pesos p){
    return regular_fitness(this->n, this->m, this->c, 100000, p);
}

pesos gen_trainer::get_max() const{
    return this->max_achieved;
}

pesos gen_trainer::randon_genome(){
    pesos p = pesos(this->param_count);
    for (uint i = 0; i < this->param_count; i++) {
        int rand_int = (int)this->__get_rand_float();
        if (i == PRIMERA_JUGADA) {
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
