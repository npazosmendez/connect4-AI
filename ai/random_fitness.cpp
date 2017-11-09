#include "fitness.hpp"
#include <vector>
#include <time.h>
#include <iomanip>      // std::setprecision

using namespace std;

vector<float> random_params(uint n, uint m, uint c){
    vector<float> p = vector<float>(golosa::cuantos_parametros(n,m,c));
    for (uint i = 0; i < p.size(); i++) {
        if (i == PRIMERA_JUGADA) {
            // estoy en PRIMERA_JUGADA
            int r = -1 + rand()%(n+1);
            p[PRIMERA_JUGADA] = float(r);
        }else{
            p[i] = rand()/float(RAND_MAX)*1000*(rand() < RAND_MAX/2 ? 1 : -1);
        }
    }
    return p;
}


int main(int argc, const char *argv[]){
    srand(time(NULL));
    uint qty,n,m,c,p;
    if (argc < 6) {
        std::cout << "params not enough" << std::endl;
        std::cout <<  "./" << argv[0] <<  " qty n m c p" << std::endl;
        exit(1);
    }else{
        qty = atoi(argv[1]);
        n = atoi(argv[2]);
        m = atoi(argv[3]);
        c = atoi(argv[4]);
        p = atoi(argv[5]);
    }

    for (uint i = 0; i < qty; i++) {
        vector<float> pesos = random_params(n,m,c);
        float r = threaded_regular_fitness(n,m,c,p,pesos, 20000);
        std::cout << "# " << i << " -> " << r  << std::endl;
        cerr << std::setprecision(5) << r << "\n";
    }
    
    return 0;
}


