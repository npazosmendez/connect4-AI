#include "gtest/gtest.h"
#include "../ai/c_linea.hpp"
#include "../ai/minimax.hpp"
#include "../ai/minimax_ab.hpp"
#include "../ai/minimax_hc.hpp"
#include <iostream>
using namespace std;

TEST(c_linea_test, gano_horizontal1) {
    c_linea juego(3,4,4,20);
    juego.jugar(1,0);
    juego.jugar(2,0);
    juego.jugar(1,1);
    juego.jugar(2,0);
    juego.jugar(1,2);
    EXPECT_TRUE(juego.termino());
    EXPECT_TRUE(juego.gano1());
}

TEST(c_linea_test, gano_vertical1) {
    c_linea juego(3,4,4,20);
    juego.jugar(1,0);
    juego.jugar(2,1);
    juego.jugar(1,0);
    juego.jugar(2,1);
    juego.jugar(1,0);
    // juego.mostrar();
    EXPECT_TRUE(juego.termino());
    EXPECT_TRUE(juego.gano1());
}

TEST(c_linea_test, gano_diagonal1) {
    c_linea juego(3,4,4,20);
    juego.jugar(1,0);
    juego.jugar(2,1);
    juego.jugar(1,1);
    juego.jugar(2,2);
    juego.jugar(1,0);
    juego.jugar(2,2);
    juego.jugar(1,2);
    // juego.mostrar();
    EXPECT_TRUE(juego.termino());
    EXPECT_TRUE(juego.gano1());
}
