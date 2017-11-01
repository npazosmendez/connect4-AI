#include "gtest/gtest.h"
#include "../ai/c_linea.hpp"
#include "../ai/minimax.hpp"
#include "../ai/minimax_ab.hpp"
#include "../ai/minimax_hc.hpp"
#include <iostream>
using namespace std;

/*
Estos tests buscan verificar que todos los minimax se comporten igual en cuanto
a victorias/derrotas sin importar sus optimizaciones.
*/
int enfrentar(c_linea juego, int (*ai1)(c_linea), int (*ai2)(c_linea));

TEST(minimax_test, normal_vs_alfabeta) {
    c_linea juego(3,4,4,20);
    EXPECT_EQ(enfrentar(juego,minimax, minimax_ab),enfrentar(juego,minimax_ab, minimax));
}
TEST(minimax_test, normal_vs_hardcore) {
    c_linea juego(3,4,4,20);
    EXPECT_EQ(enfrentar(juego,minimax, minimax_hc),enfrentar(juego,minimax_hc, minimax));
}
TEST(minimax_test, alfabeta_vs_hardcore) {
    c_linea juego(3,4,4,20);
    EXPECT_EQ(enfrentar(juego,minimax_ab, minimax_hc),enfrentar(juego,minimax_hc, minimax_ab));
}

int enfrentar(c_linea juego, int (*ai1)(c_linea), int (*ai2)(c_linea)){
    while (!juego.termino()) {
        if (juego.turno() == 1) {
            int jugada = ai1(juego);
            juego.jugar1(jugada);
        }else{
            int jugada = ai2(juego);
            juego.jugar2(jugada);
        }
    }
    if (juego.gano1()) {
        return 1;
    }else if (juego.gano2()) {
        return 2;
    }else{
        return 0;
    }
}
