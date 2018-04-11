#include "minimax.hpp"
#include "c_linea.hpp"
#include <limits>

using namespace std;
#define INFINITO numeric_limits<int>::max()

int min(c_linea &juego);
int max(c_linea &juego);

int minimax(c_linea juego){
    int val = -INFINITO;
    int res = -1;
    // para cada jugada posible
    for (int i = 0; i < juego.N; i++) {
        if (juego.tablero()[i][juego.M-1]==0) {
            int jug = juego.turno();
            juego.jugar(jug, i);
            int temp = min(juego);
            if (temp > val) {
                res = i;
                val = temp;
            }
            juego.desjugar(jug, i);
        }
    }
    return res;
};

int min(c_linea &juego){
    if (juego.termino()) {
        if (juego.gane()) {
            return 1;
        }else if(juego.perdi()){
            return -1;
        }else{
            return 0;
        }
    }
    int res = INFINITO;
    // para cada jugada posible
    for (int i = 0; i < juego.N; i++) {
        if (juego.tablero()[i][juego.M-1]==0) {
            int jug = juego.turno();
            juego.jugar(jug, i);
            res = min(res,max(juego));
            juego.desjugar(jug, i);
        }
    }
    return res;
}

int max(c_linea &juego){
    if (juego.termino()) {
        if (juego.gane()) {
            return 1;
        }else if(juego.perdi()){
            return -1;
        }else{
            return 0;
        }
    }
    int res = -1;
    // para cada jugada posible
    for (int i = 0; i < juego.N; i++) {
        if (juego.tablero()[i][juego.M-1]==0) {
            int jug = juego.turno();
            juego.jugar(jug, i);
            res = max(res,min(juego));
            juego.desjugar(jug, i);
        }
    }
    return res;
}
