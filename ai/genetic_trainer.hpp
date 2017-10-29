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
        pesos random_selection(vector<pesos> ps);
        uint fitness(pesos p);

        uint gen_limit;
        float p_mutation;
        pesos max_achieved;

    public:
        gen_trainer();
        gen_trainer(uint gl, float pm) : gen_limit(gl), p_mutation(pm){
            srand (time(NULL));
        };
        pesos train();
        pesos get_max() const;
};




#endif
