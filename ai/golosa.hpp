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
debe hacer es agregar un TU_VIEJA en el 'enum' que está acá abajo,
después de los demás ya definidos (ANTES DE PARAM_COUNT).

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
// (la 'W' se refiere a weight, pero siéntanse libres de cambiarlo)
enum{
    PRIMERA_JUGADA, /* Rango: (-1) a (N-1) DISCRETO */
    // W_FICHAS1, /* creo que tendría más sentido un peso de "progreso del juego" */
    // W_FICHAS2,
    W_DISPERSION1,  /* Rango de feature asociado: 0 - N */
    W_DISPERSION2,  /* Rango de feature asociado: 0 - N */
    W_AGRESS,   /* Rango de feature asociado: 0 ó N/2 */
    W_EXPH,  /* Rango de feature asociado: 0 ó N/2 */
    W_EXPVO,  /* Rango de feature asociado: 0 ó N/2 */
    // acá deben agregarse nuevos defines ...

    // Antes de este de abajo! No después
    PARAM_COUNT
};

/*
NOTE: la clase golosa toma los siguientes parámetros en orden:
    * todos los del enum de arriba
    * C parámetros más, indicando los pesos de las líneas
Puede usarse la función cuantos_parametros(...) para obtener el númeroe exacto.
*/

class golosa{
    friend class c_linea;
    friend class gen_trainer;
    public:

        // Constructores
        golosa(int N, int M, int C); // con parámetros basura (para testear otras cosas)
        golosa(vector<float> param, int N, int M, int C);
        golosa(int argc, char const *argv[], int N, int M, int C); // parsear argv para parámetros

        // Métodos públicos
        int jugar(c_linea juego);
        static uint cuantos_parametros(int N, int M, int C); // devuelve cuántos parámetros variables necesita la clase

        //Pongo los features publicos porque sino no se pueden testear. Igualmente seguro cambiemos todo esto
        float fila_media(const c_linea &juego, int jugador); // la media de al distribucion de las fichas por fila
        float columna_media(const c_linea &juego, int jugador); // la media de al distribucion de las fichas por columna
        inline const vector<float> &ver_parametros() { return this->parametros; }
        inline const vector<float> &ver_pesos_lineas() { return this->pesos_lineas; }
        vector<int> lineas_extensibles(const c_linea &juego, int jugador); // para 0 <= i <= C-1, indica la cantidad de líneas de longitud 'i' de color 'jugador' que sean extensibles a una de C
        bool imbatible(const c_linea &juego, int jugador); // determina si hay una línea de C-1 de 'jugador' inmediatamente extendible a ambos lados
        bool servida(const c_linea &juego); // determina si quien debe jugar ahora (turno) tiene una jugada ganadora inmediata

	    uint dispersion(const c_linea &juego, int jugador); // se fija la mayor distancia entre dos fichas de un jugador por cada linea (con al menos dos fichas de tal jugador) y las promedia

    private:

        // Variables privadas
        const vector<float> parametros; // Cantidad de params == PARAM_COUNT
        const vector<float> pesos_lineas; // Cantidad de params == C
        const int N, M, C;

        vector<float> join_params();


        // Métodos auxiliares para calcular jugada
        float puntaje(c_linea juego, int jugada_recien, int yo, int el);

        uint exp_horizontal(const c_linea &juego, int columna); // 1 o 0 segun si se colocó al lado a otra ficha del mismo jugador
        uint exp_vertical(const c_linea &juego, int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
        uint exp_oblicua(const c_linea &juego, int columna); // 1 o 0 segun si se colocó en diagonal a otra ficha del mismo jugador
        uint perjudica_rival(const c_linea &juego, int columna); // 1 o 0 segun si se colocó arriba de otra ficha del mismo jugador
        bool primera_jugada(const c_linea &juego);

        // Otros métodos auxiliares
        vector<float> leer_parametros(int argc, char const *argv[], int C);
        vector<float> leer_pesos_lineas(int argc, char const *argv[], int C);
        void print_help();
        string to_argv();
        void say_hello();

};

#endif
