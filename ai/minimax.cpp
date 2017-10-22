#include "minimax.hpp"
#include "c_linea.hpp"

int minimax(c_linea &juego){
    for (int i = 0; i < juego.N; i++) {
        if (juego.tablero()[i][juego.M-1]==0) {
            return i;
        }
    }
    return 0;
};
