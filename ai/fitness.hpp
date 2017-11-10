#ifndef FITNESS_H
#define FITNESS_H

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <cstdlib>
#include <pthread.h>

#include "golosa.hpp"

#define NUM_THREADS 8
enum{EMPATE, PRIMERO, SEGUNDO};

typedef struct params_fitness_t{
    uint n,m,c,p,its;
    vector<float> pesos;
    uint* result;
    params_fitness_t(uint n, uint m, uint c, uint p, uint its, vector<float> pesos, uint* r){
        this->n = n;
        this->m = m;
        this->c = c;
        this->p = p;
        this->its = its;
        this->pesos = std::vector<float>(pesos);
        this->result = r;
    }
}params_fitness;

using namespace std;

/*  Para medir fitness en C++ entre distintos golosos  */
uint golosa_vs_golosa(uint N, uint M, uint C, uint P, golosa &ai1, golosa &ai2);
uint golosa_vs_random(uint N, uint M, uint C, uint P, golosa &ai);
uint golosa_vs_random(uint N, uint M, uint C, uint P, golosa &golo, bool rand_primero);
list<golosa> fixture_golosas(uint N, uint M, uint C, uint P, list<golosa> aaii);
uint ida_y_vuelta(uint N, uint M, uint C, uint P, golosa &ai1, golosa &ai2);

// paralelizando cosas
void* regular_fitness_caller(void* params);
float threaded_regular_fitness(uint N, uint M, uint C, uint P, vector<float> pesos, uint trys);


/* Para medir fitness usando varias llamadas al Python */
int contar_victorias(const char* color);
string to_argv(vector<float> pesos);
uint play_with_golosa(uint N, uint M, uint C, uint P, vector<float> pesos, uint games, string rival_exec, bool me_first);
bool regular_fitness(uint N, uint M, uint C, uint P, vector<float> pesos1, vector<float> pesosmax);
float regular_fitness(uint N, uint M, uint C, uint P, vector<float> pesos);

#endif
