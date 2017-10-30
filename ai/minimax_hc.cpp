#include "minimax_hc.hpp"
#include "c_linea.hpp"
#include <climits>

using namespace std;


valor_t minimax_hc_aux(c_linea juego, valor_t alfa, valor_t beta);

int minimax_hc(c_linea &juego){
    int res = -1;
    valor_t alfa{-2,0}; // lo peor que podemos obtener
    valor_t beta{2,0}; // lo mejor que podemos obtener

    /* NOTE: en el primer nodo se debe hacer un "max". Este caso lo tratamos
    por separado para no tener que agregarle más parámetros a la función
    alfa_beta, ya que además del puntaje queremos saber la jugada, pero solo
    la de ahora.
    */
    for (int i = 0; i < juego.N; i++) {
        if (juego.tablero()[i][juego.M-1]==0) {
            if (res==-1) res=i; // si todavía no encontré ninguna opción
            juego.jugar(juego.yo, i);
            valor_t temp = minimax_hc_aux(juego, alfa, beta);
            if (temp > alfa) {
                alfa = temp;
                res = i;
            }
            juego.desjugar(juego.yo, i);
            if (beta <= alfa) break;
        }
    }
    return res;
}

valor_t minimax_hc_aux(c_linea juego, valor_t alfa, valor_t beta){
    // Caso base. Nodo terminal
    if (juego.termino()) {
        if (juego.gane())
            return {.puntos = 1, .profundidad = 1};
        else if(juego.perdi())
            return {.puntos = -1, .profundidad = 1};
        else
            return {.puntos = 0, .profundidad = 1};
    }
    // Según si me toca o no, minimizo o maximizo
    int jug = juego.turno();
    if (juego.yo == (uint)jug) {
        // Max
        // para cada jugada posible
        for (int i = 0; i < juego.N; i++) {
            if (juego.tablero()[i][juego.M-1]==0) {
                juego.jugar(jug, i);
                alfa = max(alfa, minimax_hc_aux(juego, alfa, beta));
                alfa.avanzar();
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
                beta = min(beta, minimax_hc_aux(juego, alfa, beta));
                beta.avanzar();
                juego.desjugar(jug, i);
                if (beta <= alfa) break;
            }
        }
        return beta;
    }
}
