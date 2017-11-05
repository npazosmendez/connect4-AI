#include "fitness.hpp"
#include "golosa.hpp"
#include "c_linea.hpp"
#include <vector>
#include <list>

using namespace std;


///////////////////////////////////////////////////////////////
/*  Para medir fitness en C++ de un goloso contra el random  */
///////////////////////////////////////////////////////////////


uint golosa_vs_random(uint N, uint M, uint C, uint P, golosa &ai, bool empieza_ai){
    c_linea juego(C,N,M,P);
    uint turno_ai = empieza_ai ? 1 : 2;
    while (!juego.termino()) {
        if (juego.turno() == turno_ai)
            juego.jugar1(ai.jugar(juego));
        else
            juego.jugar2(ai.jugar(juego));
    }
    if (juego.gano1())
        return PRIMERO;
    else if(juego.gano2())
        return SEGUNDO;
    else
        return EMPATE;
}

/////////////////////////////////////////////////////////
/*  Para medir fitness en C++ entre distintos golosos  */
/////////////////////////////////////////////////////////


list<golosa> fixture_golosas(uint N, uint M, uint C, uint P, list<golosa> aaii){
    /* Esta función toma una lista de golosos y crea un torneo entre ellos.
    Es un torneo con eliminación directa al perder (si se empata,
    aleatoriamente queda uno de los dos afuera). Da como resultado los
    jugadores ordenados por la tabla de posiciones (en la posición 0 el mejor).
    */
    list<golosa> posiciones;
    while (!aaii.empty()) {
        // std::cout << "quedan " << aaii.size() << " en aaii..." << std::endl;
        if (aaii.size() == 1) {
            posiciones.push_front(aaii.front());
            break;
        }
        /* Mientras la lista no esté vacía, quedan jugadores en el torneo. */
        for (list<golosa>::iterator it = aaii.begin(); it != aaii.end(); ) {
            /* Tomo los dos siguientes (si es que hay 2) y los hago jugar.
            El perdedor queda afuera del torneo y queda como siguiente en la
            tabla de posiciones. */
            list<golosa>::iterator ai1 = it;
            it++;
            if (it == aaii.end()) {
                break;
            }
            list<golosa>::iterator ai2 = it;
            it++;
            int wins1 = 0;
            int wins2 = 0;
            int match1 = golosa_vs_golosa(N,M,C,P,*ai1,*ai2); // primero ai1
            int match2 = golosa_vs_golosa(N,M,C,P,*ai2,*ai1); // primero ai2
            switch (match1) {
                case PRIMERO: wins1++; break;
                case SEGUNDO: wins2++; break;
            }
            switch (match2) {
                case PRIMERO: wins2++; break;
                case SEGUNDO: wins1++; break;
            }
            // Saco al peor del fixture y lo agrego a la tabla de posiciones
            if (wins1 < wins2) {
                posiciones.push_front(*ai1);
                aaii.erase(ai1);
            }else{
                posiciones.push_front(*ai2);
                aaii.erase(ai2);
            }
        }
    }
    return posiciones;
}

// Juega un partido donde empieza ai1
uint golosa_vs_golosa(uint N, uint M, uint C, uint P, golosa &ai1, golosa &ai2){
    c_linea juego(C,N,M,P);
    while (!juego.termino()) {
        if (juego.turno() == 1)
            juego.jugar1(ai1.jugar(juego));
        else
            juego.jugar2(ai2.jugar(juego));
    }
    if (juego.gano1())
        return PRIMERO;
    else if(juego.gano2())
        return SEGUNDO;
    else
        return EMPATE;
}


/////////////////////////////////////////////////////////
/* Para medir fitness usando varias llamadas al Python */
/////////////////////////////////////////////////////////

// TODO: esto de abajo es bastante rígido, porque depende del 'pwd'
#define BLUE_LOG_PATH "log/player_communicator_azul.log"
#define RED_LOG_PATH "log/player_communicator_rojo.log"

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

string to_argv(vector<float> pesos){
    string s;
    for (uint i = 0; i < pesos.size(); i++) {
        s += std::to_string(pesos[i]);
        if (i < pesos.size()-1) s += " ";
    }
    return s;
}


uint play_with_golosa(uint N, uint M, uint C, uint P, vector<float> pesos, uint games, string rival_exec, bool me_first) {
    /*
    PARAMETROS:
    - N, M, C, P:   Descripcion del tablero
    - pesos:        Los parametros que se le pasa a la funcion golosa
    - games:        Cantidad de partidos a Jugar
    - rival_exec:   Comando para ejecutar el codigo del oponente
    - me_first:     Indica si el goloso juega primero

    RETORNA:
    la cantidad de victorias del goloso de los games partidos
    */

    // python2 c_linea.py —blue_player ai/golosa -1 1 1 1 1 1 1 0 1 2 3 4 5 6 7 —red_player ai/minimax_ab -1 1 1 1 1 1 1 0 1 2 3 4 5 6 7 —first rojo —columns 4 —rows 4 —c 3
    assert(pesos.size() == golosa::cuantos_parametros(N,M,C));

    //INICIALMENTE BORRO EL log
    remove(BLUE_LOG_PATH);
    remove(RED_LOG_PATH);

    char command[1000];
    string color_first = me_first ? "rojo" : "azul";
    string params = to_argv(pesos);
    sprintf (command, "python ../c_linea.py --blue_player %s --red_player ./golosa %s --first %s --iterations %d --columns %d --rows %d --c %d --p %d --q True > /dev/null",
             rival_exec.c_str(), params.c_str(), color_first.c_str(), games, N, M, C, P);
    cout << command << endl;
    int code = system(command);
    if (code) exit(1);
    return contar_victorias("rojo");
}

bool regular_fitness(uint N, uint M, uint C, uint P, vector<float> pesos1, vector<float> pesosmax) {
    // RETORNA True sii el retador gano ambos encuentros

    golosa retador(pesos1, N, M, C);
    golosa maximo(pesosmax, N, M, C);
    int match1 = golosa_vs_golosa(N, M, C, P, retador, maximo); // primero ai1
    int match2 = golosa_vs_golosa(N, M, C, P, maximo, retador); // primero ai2
    return match1 == PRIMERO && match2 == SEGUNDO;
}

float regular_fitness(uint N, uint M, uint C, uint P, vector<float> pesos) {
    uint iterations_each = 25;
    string rival = "./random_player";
    uint wins_home = play_with_golosa(N,M,C,P,pesos,iterations_each,rival,true);
    uint wins_away = play_with_golosa(N,M,C,P,pesos,iterations_each,rival,false);
    // cout << wins_home << ", " << wins_away << ", " << ((float)(wins_home+wins_away))/(iterations_each*2) << endl << endl;
    return ((float)(wins_home+wins_away))/(iterations_each*2);
}
