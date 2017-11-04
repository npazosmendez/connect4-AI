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
        golosa crossover(golosa g1, golosa g2);
        void mutate(pesos &p);
        void mutate(golosa &g);
        pesos random_selection(vector<pesos> ps, vector<float> &fs);
        pesos randon_genome();
        float fitness(pesos p);

        uint n,m,c,p;
        uint gen_limit;
        uint param_count;
        float p_mutation;
        pesos max_achieved;
        float max_fitness_achieved;

        // uso interno
        string __to_argv(pesos p);
        float __get_rand_float();
        list<golosa> golosas_from_pop( vector<pesos> );

    public:
        gen_trainer();
        gen_trainer(uint _n, uint _m, uint _c, uint _p, uint gl, float pm) :
            n(_n), m(_m), c(_c), p(_p), gen_limit(gl), p_mutation(pm){
        this->param_count = golosa::cuantos_parametros(this->n, this->m, this->c);
            srand (time(NULL));
        };
        pesos train(uint pop_size);
        pesos get_max() const;
        inline float get_max_fitness(){ return this->max_fitness_achieved; };
};

#endif
