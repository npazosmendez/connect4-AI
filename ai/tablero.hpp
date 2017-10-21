#include <vector>
#include <cassert>

typedef unsigned int uint;
using namespace std;

class c_linea{
    public:
        // Constructores
        c_linea(uint C, uint N, uint M, uint p);
        c_linea(uint C, uint N, uint M, uint p, uint yo /*para indicar qué jug. soy*/);

        // Métodos para modificar el juego
        void jugar1(uint columna); // juega jug. 1
        void jugar2(uint columna); // juega jug. 2
        void jugar(uint columna); // juega jug. 'yo'

        // Métodos para observar el juego
        bool termino(); // indica si termino el juego
        bool gano1(); // indica si ganó el jug. 1
        bool gano2(); // indica si ganó el jug. 2
        bool gane(); // indica si ganó el jug. 'yo'
        uint turno() {return _turno;};
        bool metoca() {return _turno==_yo;};
        uint fichas1(){return _p1;};
        uint fichas2(){return _p2;};
        uint fichas(); // # fichas restantes del jug. 'yo'

        const vector<vector<int>>& tablero(){return _tablero;};

        const uint C;
        const uint N;
        const uint M;
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

        uint _p1; // # fichas jug 1
        uint _p2; // # fichas jug 2
        const uint _yo; // qué jug. soy
        vector<vector<int>> _tablero;
        vector<uint> _alturas;
        uint _turno;
};

c_linea::c_linea(uint C, uint N, uint M, uint p) : C(C), N(N), M(M), _p1(p), _p2(p), _yo(1), _tablero(N,vector<int>(M,0)), _alturas(N,0), _turno(1) {
    // por defecto, 'yo' = jug. 1
}
c_linea::c_linea(uint C, uint N, uint M, uint p, uint yo) : C(C), N(N), M(M), _p1(p), _p2(p), _yo(yo), _tablero(N,vector<int>(M,0)), _alturas(N,0), _turno(1) {
}

void c_linea::jugar(uint columna){
    if (_yo == 1)
        jugar1(columna);
    else
        jugar2(columna);
}

void c_linea::jugar1(uint columna){
    assert(_p1 > 0);
    assert(_alturas[columna] < M);
    assert(_turno==1);
    assert(!termino());
    _p1--;
    _tablero[columna][_alturas[columna]] = 1;
    _alturas[columna]++;
    _turno = 2;
}
void c_linea::jugar2(uint columna){
    assert(_p2 > 0);
    assert(_alturas[columna] < M);
    assert(_turno==2);
    assert(!termino());
    _p2--;
    _tablero[columna][_alturas[columna]] = 2;
    _alturas[columna]++;
    _turno = 1;
}
bool c_linea::termino(){
    // si alguien ya ganó o no hay fichas, terminó
    if(gano1() || gano2() || _p2 == 0)
        return true;
    // si están todas las columnas llenas, terminó
    for (size_t i = 0; i < N; i++) {
        if (_alturas[i] < M)
            return false;
    }
    return true;
}
bool c_linea::gano1(){
    // TODO
    return false;
}
bool c_linea::gano2(){
    // TODO
    return false;
}
bool c_linea::gane(){
    if (_yo == 1)
        return gano1();
    else
        return gano2();
}
uint c_linea::fichas(){
    if (_yo == 1)
        return fichas1();
    else
        return fichas2();
}
