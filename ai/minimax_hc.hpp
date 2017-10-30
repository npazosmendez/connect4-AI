#ifndef MINIMAX_HC_H
#define MINIMAX_HC_H

#include "c_linea.hpp"

struct valor_t{
    /* El valor de una secuencia de jugadas de minimax.
    Este struct, a diferencia de un simple valor que indique si se ganó, perdió
    o empató, permite saber qué tan profundo en el árbol está ese resultado.
    De esta manera, si el minimax determina que no puede ganar contra alguien
    perfecto, intentará jugar de tal manera de prolongar lo máximo posible
    el juego, potenciando la probabilidad de error del oponente.
    Si el oponente juega perfecto, no hay con qué darle, pero la partida durará
    lo máximo que se pueda.
    */
    int puntos;
    int profundidad;

    void avanzar(){
        this->profundidad++;
    }
    bool operator < (valor_t const &otro) const{
        return puntos < otro.puntos || (puntos == otro.puntos && profundidad < otro.profundidad);
    }

    bool operator > (valor_t const &otro) const{
        return puntos > otro.puntos || (puntos == otro.puntos && profundidad > otro.profundidad);
    }

    bool operator == (valor_t const &otro) const{
        return puntos == otro.puntos && profundidad == otro.profundidad;
    }

    bool operator <= (valor_t const &otro) const{
        return *this < otro || *this == otro;
    }

    bool operator >= (valor_t const &otro) const{
        return *this > otro || *this == otro;
    }

    valor_t static max(valor_t &uno, valor_t &otro){
        if (uno > otro) {
            return uno;
        }else{
            return otro;
        }
    }
    valor_t static min(valor_t &uno, valor_t &otro){
        if (uno < otro) {
            return uno;
        }else{
            return otro;
        }
    }
};

int minimax_hc(c_linea &juego);

#endif
