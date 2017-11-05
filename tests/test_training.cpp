#include "gtest/gtest.h"
#include "../ai/fitness.hpp"
#include "../ai/golosa.hpp"
#include "../ai/genetic_trainer/genetic_trainer.hpp"
#include <list>
using namespace std;

TEST(training_test, to_argv) {
    std::vector<float> v(5, 0);
    for(uint i=0; i<v.size(); i++){
        v[i] = i;
    }
    EXPECT_EQ(to_argv(v), "0.000000 1.000000 2.000000 3.000000 4.000000");
}

#define P_COUNT golosa::cuantos_parametros(7,5,4)
TEST(training_test, golosa_vs_golosa){
    vector<float> params_1(P_COUNT);
    vector<float> params_2(P_COUNT);
    for (uint i = 0; i < P_COUNT; i++) {
        int r = rand() * (rand() < rand() / 2 ? -1 : 1);
        if (i == 0) {
            params_1[i] = r < 0 ? -1 : rand() % 7;
            params_2[i] = r < 0 ? -1 : rand() % 7;
        }else{
            params_1[i] = rand();
            params_2[i] = rand();
        }
    }

    golosa ai1(params_1, 7 ,5 ,4);
    golosa ai2(params_2, 7 ,5 ,4);

    uint res1 = golosa_vs_golosa(7,5,4,100, ai1, ai2);
    uint res2 = golosa_vs_golosa(7,5,4,100, ai2, ai1);

    bool answer_1_ok = res1 == EMPATE || res1 == PRIMERO || res1 == SEGUNDO;
    bool answer_2_ok = res2 == EMPATE || res2 == PRIMERO || res2 == SEGUNDO;

    EXPECT_EQ(answer_1_ok, true);
    EXPECT_EQ(answer_2_ok, true);
}

#define POBLACION 10
TEST(training_test, fixture_golosas){
    vector< vector<float> > params_ps(POBLACION, vector<float>(P_COUNT));
    list<golosa> gs;
    for (int j = 0; j < POBLACION; j++) {
        for (uint i = 0; i < P_COUNT; i++) {
            int r = rand() * (rand() < rand() / 2 ? -1 : 1);
            if (i == 0) {
                params_ps[j][i] = r < 0 ? -1 : rand() % 7;
            }else{
                params_ps[j][i] = rand();
            }
        }
        gs.push_front(golosa(params_ps[j], 7 ,5 ,4));
    }
    auto fix_s = fixture_golosas(7,5,4,100, gs);

    EXPECT_EQ(fix_s.size(), params_ps.size());
}

TEST(training_test, gen_test_clipping){
    int n = 7, m = 5, c = 4;
    gen_trainer gt(n,m,c,n*m,25,0.0005);
    bool CLIP_OK = true;
    for (int i = 0; i < 50 && CLIP_OK; i++) {
        std::cout << "IT: " << i << std::endl;
        vector<float> result = gt.train(10);
        for (uint j = 0; j < result.size(); j++) {
            if (j == 0) {
                CLIP_OK &= (-1 <= result[j] && result[j] <= n-1);
            }else{
                CLIP_OK &= -1000 <= result[j] && result[j] <= 1000;
            }
            if (!CLIP_OK) {
                std::cout << "FALLO! en j = " << j << std::endl;
                std::cout << "con v[j] = " << result[j] << std::endl;
            }
        }
    }
    EXPECT_EQ(CLIP_OK, true);
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
