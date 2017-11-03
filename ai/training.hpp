#ifndef TRAINING_H
#define TRAINING_H

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>

#include "golosa.hpp"

using namespace std;

int contar_victorias(const char* color);
string to_argv(vector<float> pesos);
uint play_with_golosa(uint N, uint M, uint C, uint P, vector<float> pesos, uint games, string rival_exec, bool me_first, string root_loc);
float regular_fitness(uint N, uint M, uint C, uint P, vector<float> pesos, string root_loc);


#endif
