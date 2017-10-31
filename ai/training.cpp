#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

// TODO: esto de abajo es bastante rígido, porque depende del 'pwd'
#define BLUE_LOG_PATH "../log/player_communicator_azul.log"
#define RED_LOG_PATH "../log/player_communicator_rojo.log"

int contar_victorias(const char* color){
    /* 'color' debe ser "azul" o "rojo" */
    ifstream player_log;
    if (strcmp(color,"azul") == 0) {
        player_log.open(BLUE_LOG_PATH);
    }else if(strcmp(color, "rojo") == 0){
        player_log.open(RED_LOG_PATH);
    }else{
        cerr << "Color inválido" << endl;
        exit(1);
    }
    string palabra;
    int res = 0;
    while (player_log >> palabra) {
        if (palabra == "ganaste")
            res++;
    }
    return res;
}
