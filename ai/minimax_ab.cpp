#include "minimax.hpp"
#include "c_linea.hpp"
#include <limits>

using namespace std;

int minimax_ab_aux(c_linea juego, int alfa, int beta, int yo);

int minimax_ab(c_linea juego){
    int res = -1;
    int alfa = -1; // lo peor que podemos obtener
    int beta = 1; // lo mejor que podemos obtener
    int yo = juego.turno();

    /* NOTE: en el primer nodo se debe hacer un "max". Este caso lo tratamos
    por separado para no tener que agregarle más parámetros a la función
    alfa_beta, ya que además del puntaje queremos saber la jugada, pero solo
    la de ahora.
    */
    for (int i = 0; i < juego.N; i++) {
        if (juego.tablero()[i][juego.M-1]==0) {
            if (res==-1) res=i; // si todavía no encontré ninguna opción
            juego.jugar(yo, i);
            int temp = minimax_ab_aux(juego, alfa, beta, yo);
            if (temp > alfa) {
                alfa = temp;
                res = i;
            }
            juego.desjugar(yo, i);
            if (beta <= alfa) break;
        }
    }
    return res;
}

int minimax_ab_aux(c_linea juego, int alfa, int beta, int yo){
    // Caso base. Nodo terminal
    if (juego.termino()) {
        if (juego.gane())
            return 1;
        else if(juego.perdi())
            return -1;
        else
            return 0;
    }
    // Según si me toca o no, minimizo o maximizo
    int jug = juego.turno();
    if (yo == jug) {
        // Max
        // para cada jugada posible
        for (int i = 0; i < juego.N; i++) {
            if (juego.tablero()[i][juego.M-1]==0) {
                juego.jugar(jug, i);
                alfa = max(alfa, minimax_ab_aux(juego, alfa, beta, yo));
                juego.desjugar(jug, i);
                if (beta <= alfa) break;
            }
        }
        return alfa;
    }else{
        // Mín
        // para cada jugada posible
        for (int i = 0; i < juego.N; i++) {
            if (juego.tablero()[i][juego.M-1]==0) {
                juego.jugar(jug, i);
                beta = min(beta, minimax_ab_aux(juego, alfa, beta, yo));
                juego.desjugar(jug, i);
                if (beta <= alfa) break;
            }
        }
        return beta;
    }
}
