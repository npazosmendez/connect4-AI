#ifndef GENETIC_TRAINGER_H
#define GENETIC_TRAINGER_H

#include <time.h>
#include "golosa.hpp"
using namespace std;
typedef golosa::pesos_t pesos;

class gen_trainer{
    private:
        pesos crossover(pesos p1, pesos p2);
        void mutate(pesos &p);
        pesos random_selection(vector<pesos> ps, vector<float> &fs);
        pesos randon_genome();
        uint fitness(pesos p);

        uint n,m,c;
        uint gen_limit;
        float p_mutation;
        pesos max_achieved;

        float __get_rand_float();

    public:
        gen_trainer();
        gen_trainer(uint _n, uint _m, uint _c, uint gl, float pm) :
            n(_n), m(_m), c(_c), gen_limit(gl), p_mutation(pm){
            srand (time(NULL));
        };
        pesos train();
        pesos get_max() const;
};




#endif
