#ifndef GENETIC_TRAINGER_H
#define GENETIC_TRAINGER_H

#include <time.h>
#include "../golosa.hpp"
using namespace std;
typedef vector<float> pesos;

class gen_trainer{
    private:
        pesos crossover(pesos p1, pesos p2);
        void mutate(pesos &p);
        pesos random_selection(vector<pesos> ps, vector<float> &fs);
        pesos randon_genome();
        uint fitness(pesos p);

        uint n,m,c;
        uint gen_limit;
        uint param_count;
        float p_mutation;
        pesos max_achieved;

        // uso interno
        string __to_argv(pesos p);
        float __get_rand_float();

    public:
        gen_trainer();
        gen_trainer(uint _n, uint _m, uint _c, uint gl, float pm) :
            n(_n), m(_m), c(_c), gen_limit(gl), p_mutation(pm){
            this->param_count = golosa::cuantos_parametros(this->n, this->m, this->c);
            srand (time(NULL));
        };
        pesos train(uint pop_size);
        pesos get_max() const;
};

#endif
