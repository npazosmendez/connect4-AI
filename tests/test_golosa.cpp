#include "gtest/gtest.h"
#include "../ai/golosa.hpp"
using namespace std;

TEST(golosa_test, columna_media) {
    int C = 4; int N = 10; int M = 6; int p = 100;
    c_linea juego(C,N,M,p);
    golosa golo(golosa::pesos_t(), N, M, C, 1);

    EXPECT_EQ(golo.columna_media(juego, 1), 0);
    EXPECT_EQ(golo.columna_media(juego, 2), 0);
    juego.jugar1(3);
    juego.jugar2(1);
    EXPECT_EQ(golo.columna_media(juego, 1), 3);
    EXPECT_EQ(golo.columna_media(juego, 2), 1);
    juego.jugar1(1);
    juego.jugar2(4);
    EXPECT_EQ(golo.columna_media(juego, 1), 2);
    EXPECT_EQ(golo.columna_media(juego, 2), 5/2);
    juego.jugar1(1); //tengo 2 en la col 1 y 1 en la 3
    juego.jugar2(5);
    EXPECT_EQ(golo.columna_media(juego, 1), 5/3);
    EXPECT_EQ(golo.columna_media(juego, 2), 10/3);
}
