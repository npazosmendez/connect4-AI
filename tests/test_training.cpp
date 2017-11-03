#include "gtest/gtest.h"
#include "../ai/training.cpp"
#include "../ai/golosa.hpp"
using namespace std;

TEST(training_test, to_argv) {
    std::vector<float> v(5, 0);
    for(uint i=0; i<v.size(); i++){
        v[i] = i;
    }
    EXPECT_EQ(to_argv(v), "0.000000 1.000000 2.000000 3.000000 4.000000");
}

// TEST(training_test, play_with_golosa) {
//     uint N = 4;
//     uint M = 4;
//     uint C = 3;
//     std::vector<float> pesos(golosa::cuantos_parametros(N,M,C), 1);
//     play_with_golosa(N, M, C, 20, pesos, 10, "../minimax", true, "../");
//     play_with_golosa(N, M, C, 20, pesos, 10, "../minimax", false, "../");
//     EXPECT_GT(contar_victorias("azul"), 0);
//     EXPECT_GT(contar_victorias("rojo"), 0);
// }

// TEST(training_test, regular_fitness) {
//     uint N = 4;
//     uint M = 4;
//     uint C = 3;
//     std::vector<float> pesos(golosa::cuantos_parametros(N,M,C), 1);
//
//     cout << regular_fitness(N,M,C,20,pesos,"../") << endl;
// }
