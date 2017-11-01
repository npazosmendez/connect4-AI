#ifndef GOLOSA_H
#define GOLOSA_H

#include <vector>
#include <string>
#include "c_linea.hpp"
class c_linea;
using namespace std;
typedef unsigned int uint;


std::vector<string> string_to_argv(string);

/* ////////////////// AGREGADO DE PARÁMETROS AL GOLOSO //////////////////////
NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
Para agregar un parámetro 'TU_VIEJA' que tome el goloso, lo único que se
debe hacer es agregar un
    #define TU_VIEJA __COUNTER__
después de los demás '#defines ...' que están abajo (ANTES DE #define PARAM_COUNT).

Curiosidades a tener en cuenta:
    - El parámetro puede accederse con el vector<float> llamado 'parametros',
    haciendo parametros[TU_VIEJA].
    - El parámetro agregado, obviamente, no se utiliza solo. Probablemente
    se quiera agregar algo a la función 'puntaje(...)' que lo use.
    - Si el parámetro agregado es el peso de un feature, o similar, debe
    implementarse el método que lo calcule, y usarse como se desee. Por ejemplo
    haciendo res+=parametros[TU_VIEJA]*mi_feature(...)).
*/

// Definiciones de parámetros
#define W_FICHAS1 __COUNTER__
#define W_FICHAS2 __COUNTER__
#define W_DISPERSION __COUNTER__
#define W_AGRESS __COUNTER__
#define W_EXPH __COUNTER__
#define W_EXPV __COUNTER__
#define W_EXPO __COUNTER__

// acá deben agregarse nuevos defines ...

// Antes de este de abajo! No después
#define PARAM_COUNT __COUNTER__

class golosa{
    friend class c_linea;
    public:

        // Constructores
        golosa(int N, int M, int C, int yo); // con parámetros basura (para testear otras cosas)
        golosa(vector<float> param, int N, int M, int C, int yo);
        golosa(int argc, char const *argv[], int N, int M, int C, int yo); // parsear argv para parámetros

        // Métodos públicos
        int jugar(c_linea juego);

        //Pongo los features publicos porque sino no se pueden testear. Igualmente seguro cambiemos todo esto
        float fila_media(const c_linea &juego, int jugador); // la media de al distribucion de las fichas por fila
        float columna_media(const c_linea &juego, int jugador); // la media de al distribucion de las fichas por columna
        inline vector<float> _ver_pesos() { return this->parametros; }
    private:

        // Variables privadas
        const vector<float> parametros;

        const int N, M, C;
        const int yo;

        // Métodos auxiliares para calcular jugada
        float puntaje(c_linea &juego, int jugada_recien);

        uint contar_lineas(int contador_der, int contador_izq, int largo);
        uint lineas_nuevas(const c_linea &juego, int largo, int columna, int jugador); // # líneas de long 'largo' que se formaron en la última jugada en 'columna' (de 'jugador')
        uint exp_horizontal(const c_linea &juego, int columna); // 1 o 0 segun si se colocó al lado a otra ficha del mismo jugador
        uint exp_vertical(const c_linea &juego, int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
        uint exp_oblicua(const c_linea &juego, int columna); // 1 o 0 segun si se colocó en diagonal a otra ficha del mismo jugador
        uint perjudica_rival(const c_linea &juego, int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
        uint dispersion(const c_linea &juego, int jugador); // se fija la mayor distancia entre dos fichas de un jugador por cada linea (con al menos dos fichas de tal jugador) y las promedia

        // Otros métodos auxiliares
        vector<float> leer_pesos(int argc, char const *argv[]);
        void print_help();
        string to_argv();

};

#endif
