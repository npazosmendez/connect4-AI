#include "genetic_trainer.hpp"
#include <iostream>
#include <iomanip>
#include <string.h>

using namespace std;
void print_help();

int main(int argc, char const *argv[]) {
    /* Help, si no tengo los parámetros */
    if (argc-1 < 11) {
        print_help();
        exit(1);
    }

    /* Leo parámetros numéricos del juego y del entrenador */
    int n, m, c, p;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    c = atoi(argv[3]);
    p = atoi(argv[4]);

    int gen_limit = atoi(argv[5]);
    int pop = atoi(argv[6]);
    float p_mutation = stof(argv[7]);

    /* Leo el fitness/selection/crossover/mutation */
    gen_trainer::fitness_t fit; //8
    gen_trainer::seleccion_t sel; //9
    gen_trainer::crossover_t cross; //10
    gen_trainer::mutation_t mut; //11

    if (strcmp(argv[8], "f:vs_random") == 0){
        fit = gen_trainer::VS_RANDOM;
    }else if((strcmp(argv[8], "f:fixture") == 0)){
        fit = gen_trainer::FIXTURE;
    }else if((strcmp(argv[8], "f:vs_golosos") == 0)){
        fit = gen_trainer::VS_GOLOSOS;
    }else{
        cerr << "no conozco ese fitness: "<<argv[8] << endl;
        print_help();
        exit(1);
    }

    if (strcmp(argv[9], "s:proba") == 0){
        sel = gen_trainer::PROBABILISTICA;
    }else if((strcmp(argv[9], "s:rank") == 0)){
        sel = gen_trainer::RANK;
    }else{
        cerr << "no conozco ese selection: "<<argv[9]  << endl;
        print_help();
        exit(1);
    }

    if (strcmp(argv[10], "c:completo") == 0){
        cross = gen_trainer::COMPLETO;
    }else if((strcmp(argv[10], "c:binario") == 0)){
        cross = gen_trainer::BINARIO;
    }else{
        cerr << "no conozco ese crossover: "<<argv[10]  << endl;
        print_help();
        exit(1);
    }

    if (strcmp(argv[11], "m:mono") == 0){
        mut = gen_trainer::MONO;
    }else if((strcmp(argv[11], "m:multi") == 0)){
        mut = gen_trainer::MULTI;
    }else{
        cerr << "no conozco esa mutation: "<<argv[11]  << endl;
        print_help();
        exit(1);
    }


    /* Corro el entrenador */
    cout << "Creando entrenador..." << endl;
    gen_trainer gt = gen_trainer(n, m, c, p, gen_limit, 0.006,pop,fit,cross,sel,mut);
    gt.setMutationRate(p_mutation);
    vector<float> pesos_posta = gt.train();
    cout << "Imprimo parametros alcanzados: \n";
    for (uint i = 0; i < pesos_posta.size(); i++) {
        cout << setprecision(9) << pesos_posta[i] << " ";
    }cout << "\n";

    return 0;
}



void print_help(){
    cout << "Fallaron parámetros. Úsese como:" << endl;
    cout << "\t n m c p max_generation poblacion_inicial p_mutation <fit> <sel> <cross> <mut>" << endl;
    cout << "Donde: "<<endl;
    cout << "\t <fit>:  f:vs_random / f:fixture / f:vs_golosos" << endl;
    cout << "\t <sel>:  s:proba / s:rank" << endl;
    cout << "\t <cross>:  c:completo / c:binario" << endl;
    cout << "\t <mut>:  m:mono / m:multi" << endl;
}
