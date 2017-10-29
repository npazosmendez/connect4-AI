#include "golosa.hpp"
#include "c_linea.hpp"
#include "limits.h"
#include <string.h>

using namespace std;

// Constructores y sus parsers
golosa::golosa(pesos_t pesos, int N, int M, int C, int yo) : _pesos(pesos), N(N), M(M), C(C), yo(yo) {};

golosa::golosa(int argc, char const *argv[], int N, int M, int C, int yo) : _pesos(leer_pesos(argc, argv)), N(N), M(M), C(C), yo(yo) {};

golosa::pesos_t golosa::leer_pesos(int argc, char const *argv[]){
    // TODO: no tira errores. Lee lo que puede y lo demás queda por defecto. Solo falla si falla es stof().
    pesos_t pesos;
    for (int i = 0; i < argc; i++) {
        const char* param = argv[i];
        if (strcmp(param, "-f1") == 0 || strcmp(param, "--fichas1")) {
            pesos.fichas1 = stof(argv[i+2]);
        }else if(strcmp(param, "-f2") == 0 || strcmp(param, "--fichas2")) {
            pesos.fichas2 = stof(argv[i+1]);
        }else if(strcmp(param, "-hh") == 0 || strcmp(param, "--alturas")) {
            for (int h = 0; h < N; h++)
                pesos.alturas.push_back(stof(argv[i+1+h]));
        }else if(strcmp(param, "-d") == 0 || strcmp(param, "--dispersion")) {
            pesos.dispersion = stof(argv[i+1]);
        }else if(strcmp(param, "-a") == 0 || strcmp(param, "--agresividad")) {
            pesos.agresividad = stof(argv[i+1]);
        }else if(strcmp(param, "-eh") == 0 || strcmp(param, "--expansionh")) {
            pesos.expansion_h = stof(argv[i+1]);
        }else if(strcmp(param, "-ev") == 0 || strcmp(param, "--expansionv")) {
            pesos.expansion_v = stof(argv[i+1]);
        }else if(strcmp(param, "-eo") == 0 || strcmp(param, "--expansiono")) {
            pesos.expansion_o = stof(argv[i+1]);
        }
        // ...
    }
    return pesos;
}

void golosa::print_help(){
    /* Iba a hacer esto para indicar cómo llenar los pesos por los argumentos,
    pero el stdout de esto lo va a usar el python casi siempre. Queda en
    stany-by.
    */
}

// Métodos públicos

int golosa::jugar(c_linea juego){
    int res = -1;
    float puntaje_max;
    for (int i = 0; i < juego.N; i++) {
        // para cada jugada posible
        if (juego.tablero()[i][juego.M-1]==0) {
            juego.jugar(yo, i);
            float temp = puntaje(juego);
            if (temp > puntaje_max || res == -1) {
                res = i;
                puntaje_max = temp;
            }
            juego.desjugar(yo, i);
        }
    }
    return res;
}

// Métodos auxiliares para calcular jugada

float golosa::puntaje(c_linea &juego){
    // TODO
}


//////////////////////////////////////////////////////////////////////////////
// FUNCIONES VIEJAS. VERIFICAR QUÉ SE TIRA, QUÉ SE RECICLA, QUÉ SE PRIVATIZA, ETC.
//////////////////////////////////////////////////////////////////////////////
/*
int tira_maxima(c_linea &juego, int yo);

int puntaje(pesos_t pesos, c_linea &juego){
    // Si terminó el juego, el puntaje es extremo
    if (juego.termino()){
        if (juego.gane())
            return INT_MAX;
        else if(juego.perdi())
            return INT_MIN;
        else
            return 0; // Empate. TODO: ver cómo manejar este caso
    }

    int res = 0;

    return res;
}


int tira_maxima(c_linea &juego, int yo){
    int res = 0;
    // busco en filas
    for (int fil = 0; fil < juego.M; fil++) {
        int contador = 0;
        for (int col = 0; col < juego.N; col++) {
            contador = (juego.tablero()[col][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    // busco en columnas
    for (int col = 0; col < juego.N; col++) {
        int contador = 0;
        for (int fil = 0; fil < juego.M; fil++) {
            contador = (juego.tablero()[col][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    // busco en diagonales de 45°
    for (int col = 0; col < juego.N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < juego.M && col+fil<juego.N; fil++) {
            contador = (juego.tablero()[col+fil][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = juego.M-1; fil >= 0 && (col-(juego.M-1-fil)) >= 0; fil--) {
            contador = (juego.tablero()[col-(juego.M-1-fil)][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    // busco en diagonales de -45°
    for (int col = 0; col < juego.N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < juego.M && col-fil>=0; fil++) {
            contador = (juego.tablero()[col-fil][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = juego.M-1; fil >= 0 && (col+(juego.M-1-fil)) < juego.N; fil--) {
            contador = (juego.tablero()[col+(juego.M-1-fil)][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    return res;
}

*/
