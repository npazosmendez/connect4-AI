#include "../ai/c_linea.hpp"
#include "../ai/golosa.hpp"
#include "../ai/minimax.hpp"
#include "../ai/minimax_ab.hpp"
#include <string>
#include <cassert>
#include <string.h>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

void get_flags(int argc, char const *argv[]);
void print_help(char const *argv[]);

enum ai_t{MINIMAX, MINIMAX_AB, GOLOSA};
enum tiempo_t{TICKS,MILIS,MICROS,SECS};

uint N,M,C,P;
ai_t ai;
tiempo_t unidad;
vector<float> params;
vector<int> jugadas_previas;

int main(int argc, char const *argv[]) {
    /* Úsese --help como argumento para información */

    N = stoi(argv[1]);
    M = stoi(argv[2]);
    C = stoi(argv[3]);
    P = stoi(argv[4]);
    assert(C <= N && C <= M);
    get_flags(argc,argv);

    c_linea juego(C,N,M,P);

    // Juego las 'jugadas_previas'
    for (size_t i = 0; i < jugadas_previas.size(); i++)
        juego.jugar(juego.turno(),jugadas_previas[i]);

    if (juego.termino()){
        cerr << "Terminó el juego antes de medirse."<<endl;
        exit(1);
    }


    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;

    switch (ai) {
        case MINIMAX:
            start = high_resolution_clock::now();
            minimax(juego);
            end = high_resolution_clock::now();
            break;
        case MINIMAX_AB:
            start = high_resolution_clock::now();
            minimax_ab(juego);
            end = high_resolution_clock::now();
            break;
        case GOLOSA:
            golosa ai_golosa(params,N,M,C);
            start = high_resolution_clock::now();
            ai_golosa.jugar(juego);
            end = high_resolution_clock::now();
            break;
    }

    float tiempo;
    switch (unidad) {
        case TICKS:
            tiempo = ((duration<float>)(end-start)).count();
            break;
        case MILIS:
            tiempo = duration_cast< milliseconds >((duration<float>)(end-start)).count();
            break;
        case MICROS:
            tiempo = duration_cast< microseconds >((duration<float>)(end-start)).count();
            break;
        case SECS:
            tiempo = duration_cast< seconds >((duration<float>)(end-start)).count();
            break;
    }
    cout << tiempo << endl;

    return 0;
}

void get_flags(int argc, char const *argv[]){
    for (int i = 5; i < argc; i++) {
        const char* param = argv[i];
        if (strcmp(param, "-m") == 0){
            ai = MINIMAX;
        }else if(strcmp(param, "-mab") == 0){
            ai = MINIMAX_AB;
        }else if(strcmp(param, "--help") == 0){
            print_help(argv);
            exit(0);
        }else if(strcmp(param, "--jugadas") == 0){
            for (int j = i+1; j < argc; j++) {
                jugadas_previas.push_back(stoi(argv[j]));
            }
        }else if(strcmp(param, "-g") == 0){
            ai = GOLOSA;
            for (size_t j = i+1; j < i + 1 + golosa::cuantos_parametros(N,M,C); j++) {
                params.push_back(stof(argv[j]));
            }
        }else if(strcmp(param, "-t") == 0){
            const char* unidad_c = argv[i+1];
            if (strcmp(unidad_c, "ticks") == 0){
                unidad = TICKS;;
            }else if(strcmp(unidad_c, "milis") == 0){
                unidad = MILIS;
            }else if(strcmp(unidad_c, "micros") == 0){
                unidad = MICROS;
            }else if(strcmp(unidad_c, "secs") == 0){
                unidad = SECS;
            }
        }
    }
}

void print_help(char const *argv[]){
    cout << "Ejecútese como: " << argv[0] << " N M C P ..." << endl;
    cout <<"\t -t ticks/milis/micros/secs \t\t(determina la unidad de medición)"<< endl;
    cout <<"\t -m" << " \t\t\t\t\t(ai = minimax)" << endl;
    cout <<"\t -mab" << " \t\t\t\t\t(ai = minimax alfa beta)" << endl;
    cout <<"\t -g <p1> <p2> <p3> <p4> ..." << " \t\t(ai = goloso)" << endl;
    cout <<"\t --jugadas  <j1> <j2> <j3> <j4>..." << " \t(jugadas previas a medir)" << endl;
    cout << endl;
    cout << "IMPORTANTE: de ponerse jugadas previas, deben ir al final del comando." << endl;
}
