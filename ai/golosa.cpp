#include "golosa.hpp"
#include "c_linea.hpp"

using namespace std;

int tira_maxima(c_linea &juego, int yo);

int golosa(c_linea juego){
    int yo = juego.turno();
    // para cada jugada posible
    int tira = 0;
    int res = -1;
    for (int i = 0; i < juego.N; i++) {
        if (juego.tablero()[i][juego.M-1]==0) {
            juego.jugar(i);
            int temp = tira_maxima(juego,yo);
            if (temp > tira) {
                res = i;
                tira = temp;
            }
            juego.desjugar(yo, i);
        }
    }
    return res;}

int tira_maxima(c_linea &juego, int yo){
    int res = 0;
    // busco en filas
    for (int fil = 0; fil < juego.M; fil++) {
        int contador = 0;
        for (int col = 0; col < juego.N; col++) {
            contador = (juego.tablero()[col][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    // busco en columnas
    for (int col = 0; col < juego.N; col++) {
        int contador = 0;
        for (int fil = 0; fil < juego.M; fil++) {
            contador = (juego.tablero()[col][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    // busco en diagonales de 45°
    for (int col = 0; col < juego.N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < juego.M && col+fil<juego.N; fil++) {
            contador = (juego.tablero()[col+fil][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = juego.M-1; fil >= 0 && (col-(juego.M-1-fil)) >= 0; fil--) {
            contador = (juego.tablero()[col-(juego.M-1-fil)][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    // busco en diagonales de -45°
    for (int col = 0; col < juego.N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < juego.M && col-fil>=0; fil++) {
            contador = (juego.tablero()[col-fil][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = juego.M-1; fil >= 0 && (col+(juego.M-1-fil)) < juego.N; fil--) {
            contador = (juego.tablero()[col+(juego.M-1-fil)][fil] == yo) ? (contador+1) : 0;
            res = max(res,contador);
        }
    }
    return res;
}
