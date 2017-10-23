#include "c_linea.hpp"
#include <iostream>
#include <string>

#include "minimax.hpp"
#include "golosa.hpp"

/* Compilar usando:
    g++ -DAI=<ai> ...
    Por ejemplo: g++ -DAI=MINIMAX ai.cpp minimax.cpp -o minimax
*/

/* AI posibles: */
#define MINIMAX 1
#define MINIMAX_AB 2
#define GOLOSA 3

#ifndef AI
    #error "ERROR: AI no definida"
#endif

using namespace std;

void send(const string& msg);
void send(int msg);
int read_int(); // si encuentra "salir", exit()
string read_str(); // si encuentra "salir", exit()

int main() {

    string msg, color, oponent_color, go_first;
    int columns, rows, c, p, yo, el;

    // Tantos juegos como el juez diga
    while (true) {
        // No sé por qué me importan los colores pero como ud diga sr juez
        color = read_str();
        oponent_color = read_str();

        // Qué forma tiene el juego?
        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();

        // Qué jugador soy?
        go_first = read_str();
        if (go_first == "vos"){
            yo = 1;
            el = 2;
        }else{
            yo = 2;
            el = 1;
        }

        // Acá empieza el juego
        int jugada;
        c_linea juego(c,columns,rows,p,yo);
        if (yo==1) {
            // me toca primero
            #if AI==MINIMAX
                jugada = minimax(juego);
                juego.jugar(yo,jugada);
            #elif AI==GOLOSA
                jugada = golosa(juego);
                juego.jugar(yo,jugada);
            #endif
            send(jugada);
        }
        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron")
                break;
            else
                juego.jugar(el, stoi(msg));

            #if AI==MINIMAX
                jugada = minimax(juego);
                juego.jugar(yo,jugada);
            #elif AI==MINIMAX_AB
                // ...
            #elif AI==GOLOSA
                jugada = golosa(juego);
                juego.jugar(yo,jugada);
            #endif
            send(jugada);
        }
    }
    return 0;
}


/////////////////////
//*  Auxiliares    */
/////////////////////


void send(const string& msg) {
    cout << msg << endl;
}

void send(int msg) {
    cout << msg << endl;
}

int read_int() {
    string msg;
    cin >> msg;
    if (msg == "salir") {
        send("listo");
        exit(0);
    }
    return stoi(msg);
}

string read_str() {
    string msg;
    cin >> msg;
    if (msg == "salir") {
        send("listo");
        exit(0);
    }
    return msg;
}
