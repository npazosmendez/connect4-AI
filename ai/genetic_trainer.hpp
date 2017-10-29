#ifndef GENETIC_TRAINGER_H
#define GENETIC_TRAINGER_H

#include <time.h>
#include "golosa.hpp"
using namespace std;
typedef golosa::pesos_t pesos;

class gen_trainer{
    private:
        pesos crossover(pesos p1, pesos p2);
        pesos mutate(pesos p);
        pesos random_selection(vector<pesos> ps);
        uint fitness(pesos p);

        uint gen_limit;
        pesos max_achieved;

    public:
        gen_trainer();
        gen_trainer(uint gl) : gen_limit(gl){
            srand (time(NULL));
        };
        pesos train();
        pesos get_max() const;
};




#endif
