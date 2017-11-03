#include "genetic_trainer.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc-1 < 5) {
        std::cout << "faltan parametros" << std::endl;
        std::cout << "n m c max_generation poblacion_inicial" << std::endl;
        exit(1);
    }

    int n, m, c;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    c = atoi(argv[3]);
    int gen_limit = atoi(argv[4]);
    int pop = atoi(argv[5]);

    std::cout << "Creando entrenador..." << std::endl;
    gen_trainer gt = gen_trainer(4, 4, 3, gen_limit, 0.05);
    std::cout << "Entrenando..." << std::endl;
    auto pesos_posta = gt.train(pop);
    std::cout << "imprimo parametrod: ";
    for (uint i = 0; i < pesos_posta.size(); i++) {
        std::cout << pesos_posta[i] << "  " << std::endl;
        
    }
    std::cout << "FITNESS MAX ALCANZADO: " << gt.get_max_fitness() << std::endl;
    std::cout << "los pesos tienen largo " << pesos_posta.size() << std::endl;


    return 0;
}


