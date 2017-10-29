#include "c_linea.hpp"
#define ASSERT

c_linea::c_linea(uint C, uint N, uint M, uint p) : C(C), N(N), M(M), yo(1), _p1(p), _p2(p), _tablero(N,vector<int>(M,0)), _alturas(N,0), _turno(1) {
    // por defecto, 'yo' = jug. 1
}

c_linea::c_linea(uint C, uint N, uint M, uint p, uint yo_v) : C(C), N(N), M(M), yo(yo_v), _p1(p), _p2(p), _tablero(N,vector<int>(M,0)), _alturas(N,0), _turno(1) {
    #ifdef ASSERT
    assert(yo==1 || yo==2);
    #endif
}

void c_linea::jugar(int jugador, int columna){
    #ifdef ASSERT
    assert(columna < N);
    assert(jugador==1 || jugador==2);
    assert(_alturas[columna] < M);
    assert(_turno==(uint)jugador);
    assert(!termino());
    #endif
    if (jugador==1){
        _p1--;
        _turno = 2;
    }else{
        _p2--;
        _turno = 1;
    }
    _tablero[columna][_alturas[columna]] = jugador;
    _alturas[columna]++;
}

void c_linea::desjugar(int jugador, int columna){
    #ifdef ASSERT
    assert(columna < N);
    assert(jugador==1 || jugador==2);
    assert(_alturas[columna] > 0);
    assert(_tablero[columna][_alturas[columna]-1]==jugador);
    #endif
    if (jugador==1){
        assert(_turno==2);
        _p1++;
        _turno = 1;
    }else{
        assert(_turno==1);
        _p2++;
        _turno = 2;
    }
    _alturas[columna]--;
    _tablero[columna][_alturas[columna]] = 0;
}

bool c_linea::termino(){
    // si alguien ya ganó o no hay fichas, terminó
    if(gano1() || gano2() || _p2 == 0)
        return true;
    // si están todas las columnas llenas, terminó
    for (int i = 0; i < N; i++) {
        if (_alturas[i] < M)
            return false;
    }
    return true;
}

bool c_linea::gano(int jugador){
    assert(jugador == 1 || jugador ==2);
    // busco en filas
    for (int fil = 0; fil < M; fil++) {
        int contador = 0;
        for (int col = 0; col < N; col++) {
            contador = (_tablero[col][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    // busco en columnas
    for (int col = 0; col < N; col++) {
        int contador = 0;
        for (int fil = 0; fil < M; fil++) {
            contador = (_tablero[col][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    // busco en diagonales de 45°
    for (int col = 0; col < N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < M && col+fil<N; fil++) {
            contador = (_tablero[col+fil][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col-(M-1-fil)) >= 0; fil--) {
            contador = (_tablero[col-(M-1-fil)][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    // busco en diagonales de -45°
    for (int col = 0; col < N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < M && col-fil>=0; fil++) {
            contador = (_tablero[col-fil][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col+(M-1-fil)) < N; fil--) {
            contador = (_tablero[col+(M-1-fil)][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    return false;
}


void c_linea::mostrar(){
    for (int fil = M-1; fil >= 0; fil--){
        for (int col = 0; col < N; col++){
            cout << _tablero[col][fil] << " ";
        }
        cout << endl;
    }
}
