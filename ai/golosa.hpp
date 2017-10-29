#ifndef GOLOSA_H
#define GOLOSA_H

#include <vector>
#include "c_linea.hpp"

class c_linea;
using namespace std;

class golosa{
    friend class c_linea;
    public:
        struct pesos_t;

        // Constructores
        golosa(pesos_t pesos, int N, int M, int C, int yo);
        golosa(int argc, char const *argv[], int N, int M, int C, int yo); // parsear argv para pesos

        // Métodos públicos
        int jugar(c_linea juego);

        // Posibles features (sujeto a modificaciones)
        struct pesos_t{
            float fichas1; // -f1 --fichas1
            float fichas2; // -f2 --fichas2
            float dispersion; // -d --dispersion
            float agresividad; // -a --agresividad
            float expansion_h; // -eh -expansionh
            float expansion_v; // -ev -expansionv
            float expansion_o; // -eo -expansiono
            vector<float> alturas; // -hh --alturas
            // Cantidad de rectas de long X (1<=X<=C) (extensibles)
            // TODO: a definir

            // Valores por defecto
            pesos_t() : fichas1(1), fichas2(1), dispersion(1), agresividad(1), expansion_h(1), expansion_v(1), expansion_o(1) {};
        };

    private:

        // Variables privadas
        const pesos_t _pesos;
        const int N, M, C;
        const int yo;

        // Métodos auxiliares para calcular jugada
        float puntaje(c_linea &juego);

        // Otros métodos auxiliares
        pesos_t leer_pesos(int argc, char const *argv[]);
        void print_help();

};

#endif
