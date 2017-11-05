#ifndef C_LINEA_H
#define C_LINEA_H

#include <vector>
#include <cassert>
#include <iostream>
#include "golosa.hpp"

typedef unsigned int uint;
using namespace std;
class golosa;

class c_linea{
    friend class golosa;
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
        void paso(){if (_turno==1) _turno = 2; else _turno = 1;};


        // Métodos para observar el juego
        const vector< vector<int> >& tablero() const {return _tablero;};
        bool termino(); // indica si terminó el juego
        bool gano(int jugador); // indica si ganó el jugador 'jugador'
        bool gano1(){return gano(1);};
        bool gano2(){return gano(2);};
        bool gane(){return gano(yo);};
        bool perdio(int jugador){if (jugador == 1) return gano2(); else return gano1();};
        bool perdio1(){return perdio(1);};
        bool perdio2(){return perdio(2);};
        bool perdi(){return perdio(yo);};
        uint turno() const {return _turno;};
        bool metoca() const {return _turno==yo;};
        uint fichas1()const {return _p1;};
        uint fichas2() const{return _p2;};
        uint fichas() const {if (yo == 1) return fichas1(); else return fichas2();};

        int altura(int col) {return _alturas[col];};
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


};

#endif
