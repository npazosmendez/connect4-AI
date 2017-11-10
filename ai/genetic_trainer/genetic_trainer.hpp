#ifndef GENETIC_TRAINGER_H
#define GENETIC_TRAINGER_H

#include <list>
#include <time.h>
#include "../golosa.hpp"
using namespace std;

class gen_trainer{
    private:
        // Crossovers
        vector<float>  crossover_binario(vector<float>  p1, vector<float>  p2);
        vector<float>  crossover_completo(vector<float>  p1, vector<float>  p2);

        // Mutations
        bool mono_mutate(vector<float>  &p);
        bool multi_mutate(vector<float>  &p);

        // Selections
        void progenitores_por_fitness(vector<float> &p1, vector<float> &p2, const vector<float> &pop_fitness, const vector< vector<float> > &pop);
        void progenitores_probabilisticos(vector<float> &p1, vector<float> &p2, const vector<float> &pop_fitness, const vector< vector<float> > &pop);
        vector<float>  random_selection(vector<vector<float> > ps, vector<float> &fs);

        // Fitness
        float fitness_against_random(vector<float>  p);
        float fitness_against_golosos(vector<float> &chabon);
        vector<float> fitness_fixture(const vector< vector<float> > &pop);

        // Otros
        vector<float>  randon_genome();
        
        uint n,m,c,p;
        uint gen_limit;
        uint param_count;
        float p_mutation;
        uint pop_size;


    public:
        gen_trainer();
        gen_trainer(uint _n, uint _m, uint _c, uint _p, uint gl, float pm) :
            n(_n), m(_m), c(_c), p(_p), gen_limit(gl), p_mutation(pm), pop_size(50){
            this->param_count = golosa::cuantos_parametros(this->n, this->m, this->c);
                srand (time(NULL));
        };
        gen_trainer(uint _n, uint _m, uint _c, uint _p, uint gl, float pm, uint pop_size) :
            n(_n), m(_m), c(_c), p(_p), gen_limit(gl), p_mutation(pm), pop_size(pop_size){
            this->param_count = golosa::cuantos_parametros(this->n, this->m, this->c);
                srand (time(NULL));
        };
        vector<float>  train(uint pop_size);
        vector<float>  train();

        void setPopulationSize(uint p){
            this->pop_size = p;
        }
        uint getPopulationSize(){
            return this->pop_size;
        }
        void setMutationRate(float p_mut){
            this->p_mutation = p_mut;
        }
        float getMutationRate(){
            return this->p_mutation;
        }

};

#endif
