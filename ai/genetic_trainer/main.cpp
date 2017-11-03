#include "genetic_trainer.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

    std::cout << "Creando entrenador..." << std::endl;
    gen_trainer gt = gen_trainer(4, 4, 3, 100, 0.05);
    std::cout << "Entrenando..." << std::endl;
    auto pesos_posta = gt.train(10);
    std::cout << "los pesos tienen largo " << pesos_posta.size() << std::endl;


    return 0;
}


