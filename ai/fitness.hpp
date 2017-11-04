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

#include "golosa.hpp"

enum{EMPATE, PRIMERO, SEGUNDO};

using namespace std;

/*  Para medir fitness en C++ entre distintos golosos  */
uint golosa_vs_golosa(uint N, uint M, uint C, uint P, golosa &ai1, golosa &ai2);
uint golosa_vs_random(uint N, uint M, uint C, uint P, golosa &ai);
list<golosa> fixture_golosas(uint N, uint M, uint C, uint P, list<golosa> aaii);

/* Para medir fitness usando varias llamadas al Python */
int contar_victorias(const char* color);
string to_argv(vector<float> pesos);
uint play_with_golosa(uint N, uint M, uint C, uint P, vector<float> pesos, uint games, string rival_exec, bool me_first);
float regular_fitness(uint N, uint M, uint C, uint P, vector<float> pesos);


#endif
