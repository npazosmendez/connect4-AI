#ifndef GENETIC_TRAINGER_H
#define GENETIC_TRAINGER_H

#include <list>
#include <time.h>
#include "../golosa.hpp"
using namespace std;
typedef vector<float> pesos;

class gen_trainer{
    private:
        pesos crossover(pesos p1, pesos p2);
        void mutate(pesos &p);
        pesos random_selection(vector<pesos> ps, vector<float> &fs);
        pesos random_selection(vector<pesos> ps, vector<float> &fs, float& f_th);
        pesos randon_genome();
        float fitness_against_random(pesos p);

        uint n,m,c,p;
        uint gen_limit;
        uint param_count;
        float p_mutation;
        uint pop_size;

        // uso interno
        string __to_argv(pesos p);
        float __get_rand_float();
        vector<float> clip_float_values(vector<float>);
        list<golosa> golosas_from_pop( vector<pesos> );

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
        pesos train(uint pop_size);
        pesos train_random_fitness();

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
