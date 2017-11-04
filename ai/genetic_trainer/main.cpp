#include "genetic_trainer.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc-1 < 6) {
        std::cout << "faltan parametros" << std::endl;
        std::cout << "n m c p max_generation poblacion_inicial" << std::endl;
        exit(1);
    }

    int n, m, c, p;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    c = atoi(argv[3]);
    p = atoi(argv[4]);

    int gen_limit = atoi(argv[5]);
    int pop = atoi(argv[6]);

    std::cout << "Creando entrenador..." << std::endl;
    gen_trainer gt = gen_trainer(n, m, c, p, gen_limit, 0.005);
    std::cout << "Entrenando..." << std::endl;
    auto pesos_posta = gt.train(pop);
    std::cout << "imprimo parametros alcanzados: ";
    for (uint i = 0; i < pesos_posta.size(); i++) {
        std::cout << pesos_posta[i] << "  " << std::endl;
    }
    return 0;
}


