#ifndef C_LINEA_H
#define C_LINEA_H

#include <vector>
#include <cassert>
#include <iostream>

typedef unsigned int uint;
using namespace std;

class c_linea{
    public:
        // Constructores
        c_linea(uint C, uint N, uint M, uint p);
        c_linea(uint C, uint N, uint M, uint p, uint yo /*indica qué jug. soy*/);

        // Métodos para modificar el juego
        void jugar(int jugador, int columna); // juega 'jugador' en 'columna'
        void jugar1(uint columna){jugar(1,columna);};
        void jugar2(uint columna){jugar(2,columna);};
        void jugar(uint columna){jugar(yo,columna);};
        void desjugar(int jugador, int columna); // desjuega 'jugador' en 'columna'
        void desjugar1(uint columna){desjugar(1,columna);};
        void desjugar2(uint columna){desjugar(2,columna);};
        void desjugar(uint columna){desjugar(yo,columna);};


        // Métodos para observar el juego
        const vector< vector<int> >& tablero(){return _tablero;};
        bool termino(); // indica si terminó el juego
        bool gano(int jugador); // indica si ganó el jugador 'jugador'
        bool gano1(){return gano(1);};
        bool gano2(){return gano(2);};
        bool gane(){return gano(yo);};
        bool perdio(int jugador){if (jugador == 1) return gano2(); else return gano1();};
        bool perdio1(){return perdio(1);};
        bool perdio2(){return perdio(2);};
        bool perdi(){return perdio(yo);};
        uint turno() {return _turno;};
        bool metoca() {return _turno==yo;};
        uint fichas1(){return _p1;};
        uint fichas2(){return _p2;};
        uint fichas(){if (yo == 1) return fichas1(); else return fichas2();};

        // Métodos para las tácticas golosas
        uint lineas_nuevas(int largo, int columna, int jugador); // # líneas de long 'largo' que se formaron en la última jugada en 'columna' (de 'jugador')
	uint exp_horizontal(int columna); // 1 o 0 segun si se colocó al lado a otra ficha del mismo jugador
	uint exp_vertical(int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
	uint exp_oblicua(int columna); // 1 o 0 segun si se colocó en diagonal a otra ficha del mismo jugador
	uint perjudica_rival(int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
	uint dispercion(int jugador); // se fija la mayor distancia entre dos fichas de un jugador por cada linea (con al menos dos fichas de tal jugador) y las promedia

        // Variables públicas
        const int C;
        const int N;
        const int M;
        const uint yo; // qué jug. soy

        void mostrar(); // muestra por consola el tablero. para debugear

    private:
        /* El tablero es una matriz (arreglo de columnas), y cada posición
        puede tener:
            0 : no hay ficha
            1: hay ficha de jug. 1
            2: hay ficha de jug. 2
        Ejemplo:
            0 0 0 0 0 0
            0 0 1 1 0 0
            1 2 2 1 2 0
            2 1 2 1 2 0
        En _alturas se guarda la altura de cada columna, que en realidad está
        implícita en la info de la matriz. */

        // Variables privadas
        uint _p1; // # fichas jug 1
        uint _p2; // # fichas jug 2
        vector< vector<int> > _tablero;
        vector<int> _alturas;
        uint _turno;

        // Auxiliares
        uint contar_lineas(int contador_der, int contador_izq, int largo);

};

#endif
