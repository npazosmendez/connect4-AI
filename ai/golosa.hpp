#ifndef GOLOSA_H
#define GOLOSA_H

#include <vector>
#include "c_linea.hpp"
#define PESOS_COUNT 7

class c_linea;
using namespace std;
typedef unsigned int uint;


vector<float> pesos_to_vec(golosa::pesos_t p);

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
        // -------------------------------------------------------
        //                       IMPORTANTE !
        // -------------------------------------------------------
        // NOTE: Si agregan algun peso, dejen el vector alturas al final,
        // y actualicen el define PESOS_COUNT
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
        float puntaje(c_linea &juego, int jugada_recien);

        uint contar_lineas(int contador_der, int contador_izq, int largo);
        uint lineas_nuevas(const c_linea &juego, int largo, int columna, int jugador); // # líneas de long 'largo' que se formaron en la última jugada en 'columna' (de 'jugador')
        uint exp_horizontal(const c_linea &juego, int columna); // 1 o 0 segun si se colocó al lado a otra ficha del mismo jugador
        uint exp_vertical(const c_linea &juego, int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
        uint exp_oblicua(const c_linea &juego, int columna); // 1 o 0 segun si se colocó en diagonal a otra ficha del mismo jugador
        uint perjudica_rival(const c_linea &juego, int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
        uint dispersion(const c_linea &juego, int jugador); // se fija la mayor distancia entre dos fichas de un jugador por cada linea (con al menos dos fichas de tal jugador) y las promedia
        uint fila_media(const c_linea &juego, int jugador); // la media de al distribucion de las fichas por fila
        uint columna_media(const c_linea &juego, int jugador); // la media de al distribucion de las fichas por columna

        // Otros métodos auxiliares
        pesos_t leer_pesos(int argc, char const *argv[]);
        void print_help();

};

#endif
