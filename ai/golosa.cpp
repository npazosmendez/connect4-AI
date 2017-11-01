#include "golosa.hpp"
#include "c_linea.hpp"
#include "limits.h"
#include <string.h>
#include <string>

using namespace std;


int golosa::jugar(c_linea juego){
    /* Dentro de las jugadas posibles, esta función elige la que lleva al juego
    al mejor tablero posible para el jugador, cuyo 'puntaje' se determina
    con la función de tal nombre, combinando features del tablero. */
    int res = -1;
    float puntaje_max;
    for (int i = 0; i < juego.N; i++) {
        // para cada jugada posible
        if (juego.tablero()[i][juego.M-1]==0) {
            juego.jugar(yo, i);
            float temp = puntaje(juego,i);
            if (temp > puntaje_max || res == -1) {
                res = i;
                puntaje_max = temp;
            }
            juego.desjugar(yo, i);
        }
    }
    return res;
}

float golosa::puntaje(c_linea &juego, int jugada_recien){
    float res = 0;

    // TODO

    return res;
}


/* /////////////////////////////////////// */
// FEATURES Y DEMÁS DE UN JUEGO PARTICULAR //
/* /////////////////////////////////////// */

uint golosa::contar_lineas(int contador_der, int contador_izq, int largo){
    /* Auxiliar para lineas_nuevas(...):
    Dadas las cantidades de fichas a izquierda y derecha de igual color de una
    ficha central, y el largo de la línea a formar, cuenta cuántas lineas hay
    de tal largo, que cumplan:
        * contienen a la ficha central
        * al menos uno de sus extremos es límite (no está contenida en otra más grande hacia ambos lados)
    */
    uint res = 0;
    if (contador_der +1 == largo || (contador_der+1 > largo && contador_izq == 0)) res++; // encontré a derecha
    if (contador_izq +1 == largo || (contador_izq+1 > largo && contador_der== 0)) res++; // encontré a izquierda
    if ((contador_izq+1 < largo || contador_der +1 < largo) && contador_der > 0 && contador_izq > 0) {
        // si para algún lado hay algo pero no llegué, me fijo si puedo extenderme hacia los dos lados
        // por ejemplo, lineas de 3: ooox(X)xxooo, no llegué a izquierda pero puedo extenderme hacia ambos lados
        if (contador_der + contador_izq + 1 >= largo) res++;
    }
    return res;
}

uint golosa::lineas_nuevas(const c_linea &juego, int largo, int columna, int jugador){
    /* NOTE
    Esta función devuelve la cantidad de líneas de longitud 'largo' que cumplen:
        * son de 'jugador'
        * contienen a la ficha de más arriba de 'columna'
        * al menos uno de sus extremos es límite (no está contenida en otra más grande hacia ambos lados)

    La idea de esta función es ver cuántas líneas de largo 'largo' me agrega
    haber jugado en 'columna'. La decisión de contar solo las líneas que tienen
    algún extremo límite surge de que, a primera vista, no parecería
    interesante contar líneas que en realidad son más largas para ambos lados.
    Interesa contar aquellas líneas de largo L, que en un futuro puedan llegar
    a ser de largo L+1.

    TODO: tal vez tenga sentido que la función tenga en
    cuenta que la línea contada pueda ser expandida. Si hacemos eso, mejor
    hacerlo en otra función, y conservar esta.
    */
    #ifdef ASSERT
    assert(jugador==1 || jugador==2);
    assert(largo > 0);
    assert(columna < N);
    assert(juego._alturas[columna] > 0);
    assert(juego.tablero()[columna][juego._alturas[columna]-1]==jugador);
    #endif

    uint res = 0;

    // busco en columna (para abajo nada más)
    int contador = 0;
    int fila = juego._alturas[columna]-1;
    while(fila >= 0 && juego.tablero()[columna][fila]==jugador && contador < largo){
        contador++;
        fila--;
    }
    if (contador == largo) res++; // encontré una
    // cout << "En columna: " << res << endl;

    // busco en fila (hacia ambos lados)
    // cuento a derecha
    int contador_der = 0;
    int col = columna+1;
    while(col < N && juego.tablero()[col][juego._alturas[columna]-1]==jugador){
        contador_der++;
        col++;
    }
    // cuento a izquierda
    int contador_izq = 0;
    col = columna-1;
    while(col >= 0 && juego.tablero()[col][juego._alturas[columna]-1]==jugador){
        contador_izq++;
        col--;
    }
    res += contar_lineas(contador_der, contador_izq, largo);
    // cout << "En fila: " << res << endl;

    // busco a 45° (hacia ambos lados)
    // cuento a derecha (hacia arriba)
    contador_der = 0;
    col = columna+1;
    fila = juego._alturas[columna]-1 +1;
    while(col < N && fila < M && juego.tablero()[col][fila]==jugador){
        contador_der++;
        col++;
        fila++;
    }
    // cuento a izquierda (hacia abajo)
    contador_izq = 0;
    col = columna-1;
    fila = juego._alturas[columna]-1 -1;
    while(col >= 0 && fila >= 0 && juego.tablero()[col][fila]==jugador){
        contador_izq++;
        col--;
        fila--;
    }
    res += contar_lineas(contador_der, contador_izq, largo);
    // cout << "En 45°: " << res << endl;

    // busco a -45° (hacia ambos lados)
    // cuento a derecha (hacia abajo)
    contador_der = 0;
    col = columna+1;
    fila = juego._alturas[columna]-1 -1;
    while(col < N && fila >= 0 && juego.tablero()[col][fila]==jugador){
        contador_der++;
        col++;
        fila--;
    }
    // cuento a izquierda (hacia arriba)
    contador_izq = 0;
    col = columna-1;
    fila = juego._alturas[columna]-1 +1;
    while(col >= 0 && fila < M && juego.tablero()[col][fila]==jugador){
        contador_izq++;
        col--;
        fila++;
    }
    res += contar_lineas(contador_der, contador_izq, largo);
    // cout << "En -45°: " << res << endl;

    return res;
}

uint golosa::exp_horizontal(const c_linea &juego, int col){
	int fila=juego._alturas[col]-1;
	if (col-1>=0&&col+1<N)
		return (juego.tablero()[col-1][fila]==juego.tablero()[col][fila]) || (juego.tablero()[col+1][fila]==juego.tablero()[col][fila]);
	if (col-1<0)
		return (juego.tablero()[col+1][fila]==juego.tablero()[col][fila]);
	if (col+1>=N)
		return (juego.tablero()[col-1][fila]==juego.tablero()[col][fila]);
	return 0;
}

uint golosa::exp_vertical(const c_linea &juego, int col){//no puedo colocar una ficha debajo de otra, asi que solo importa ver hacia abajo
	int fila=juego._alturas[col]-1;
	if (fila-1<0)
		return false;
	else
		return (juego.tablero()[col][fila]==juego.tablero()[col][fila-1]);
}

uint golosa::exp_oblicua(const c_linea &juego, int col){
	int fila=juego._alturas[col]-1;
	bool hay=false;
	if (col-1>=0&&fila+1<M)
		hay =hay || (juego.tablero()[col-1][fila+1]==juego.tablero()[col][fila]);
	if (col-1>=0&&fila-1>=0)
		hay =hay || (juego.tablero()[col-1][fila-1]==juego.tablero()[col][fila]);
	if (col+1<N&&fila+1<M)
		hay =hay || (juego.tablero()[col+1][fila+1]==juego.tablero()[col][fila]);
	if (col+1<N&&fila-1>=0)
		hay =hay || (juego.tablero()[col+1][fila-1]==juego.tablero()[col][fila]);
	return hay;
}

uint golosa::perjudica_rival(const c_linea &juego, int col){
	int fila=juego._alturas[col]-1;
	bool hayAdyEnemigo=false;
	bool hayAdyMio=false;
	if (col-1>=0&&fila+1<M)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col-1][fila+1]!=juego.tablero()[col][fila])&&juego.tablero()[col-1][fila+1]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col-1][fila+1]==juego.tablero()[col][fila]);
	}
	if (col-1>=0&&fila-1>=0)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col-1][fila-1]!=juego.tablero()[col][fila])&&juego.tablero()[col-1][fila-1]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col-1][fila-1]==juego.tablero()[col][fila]);
	}
	if (col+1<N&&fila+1<M)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col+1][fila+1]!=juego.tablero()[col][fila])&&juego.tablero()[col+1][fila+1]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col+1][fila+1]==juego.tablero()[col][fila]);
	}
	if (col+1<N&&fila-1>=0)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col+1][fila-1]!=juego.tablero()[col][fila])&&juego.tablero()[col+1][fila-1]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col+1][fila-1]==juego.tablero()[col][fila]);
	}
	if (col+1<N)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col+1][fila]!=juego.tablero()[col][fila])&&juego.tablero()[col+1][fila]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col+1][fila]==juego.tablero()[col][fila]);
	}
	if (col-1>=0)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col-1][fila]!=juego.tablero()[col][fila])&&juego.tablero()[col-1][fila]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col-1][fila]==juego.tablero()[col][fila]);
	}
	if (fila+1<N)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col][fila+1]!=juego.tablero()[col][fila])&&juego.tablero()[col][fila+1]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col][fila+1]==juego.tablero()[col][fila]);
	}
	if (fila-1>=0)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((juego.tablero()[col][fila-1]!=juego.tablero()[col][fila])&&juego.tablero()[col][fila-1]!=0);
		hayAdyMio = hayAdyMio || (juego.tablero()[col][fila-1]==juego.tablero()[col][fila]);
	}
	return (hayAdyEnemigo&&!hayAdyMio);
}

uint golosa::dispersion(const c_linea &juego, int jugador){
	int distFila[M];
	for(int i=0;i<M;i++)//inicializo en 0
		distFila[i]=0;
	for(int f=0;f<M;f++){//para cada fila
		int pos1=N;
		int pos2=-1;
		bool hay=false;
		for(int c=0;c<N;c++){//recorro las columnas
			if(juego.tablero()[c][f]==jugador){
				hay=true;
				if(c<pos1)//minimizo pos1
					pos1=c;
				if(c>pos2)//maximizo pos2
					pos2=c;
			}
		}
		if(hay)
			distFila[f]=pos2-pos1;//la distancia entre las dos fichas mas alejadas de esa fila
	}
	int total=0;
	int sumados=0;
	for(int i=0;i<M;i++){
		if(distFila[i]!=0){
			total+=distFila[i];
			sumados++;
		}
	}
	return ((uint)(total/sumados));
}

float golosa::fila_media(const c_linea &juego, int jugador){
    // la media de al distribucion de las fichas por fila
    int cant_por_fila[M];  //Me guardo la cantidad de fichas de cada fila
    for (int i = 0; i < M; i++ ) { //Lo inicializo en 0
        cant_por_fila[i] = 0;
    }
    //Recorro todo el tablero buscando fichas del  jugador y las vos contando por buckets
    for (int fila = 0; fila < M; fila++ ) {
        for (int col = 0; col < N; col++ ) {
            if (juego.tablero()[col][fila] == jugador)
                cant_por_fila[fila]++;
        }
    }
    int sumatoria_ponderada = 0;
    int fichas_usadas = 0;
    for (int i = 0; i < M; i++ ) {
        fichas_usadas += cant_por_fila[i];
        sumatoria_ponderada += i * cant_por_fila[i];
    }
    return fichas_usadas==0 ? 0 : sumatoria_ponderada/fichas_usadas;
}
float golosa::columna_media(const c_linea &juego, int jugador){
    // la media de al distribucion de las fichas por columna
    int cant_por_columna[N];  //Me guardo la cantidad de fichas de cada columna
    for (int i = 0; i < N; i++ ) { //Lo inicializo en 0
        cant_por_columna[i] = 0;
    }
    //Recorro todo el tablero buscando fichas del  jugador y las vos contando por buckets
    for (int fila = 0; fila < M; fila++ ) {
        for (int col = 0; col < N; col++ ) {
            if (juego.tablero()[col][fila] == jugador)
                cant_por_columna[col]++;
        }
    }
    int sumatoria_ponderada = 0;
    int fichas_usadas = 0;
    for (int i = 0; i < N; i++ ) {
        fichas_usadas += cant_por_columna[i];
        sumatoria_ponderada += i * cant_por_columna[i];
    }
    return fichas_usadas==0 ? 0 : sumatoria_ponderada/fichas_usadas;
}



/* /////////////////////////////////////// */
//   AUXILIARES DE INICIALIZACIÓN Y OTROS  //
//   (NO TIENEN QUE VER CON EL JUGADOR)    //
/* /////////////////////////////////////// */


// Constructores y sus parsers
golosa::golosa(int N, int M, int C, int yo) : parametros(PARAM_COUNT), N(N), M(M), C(C), yo(yo) {}

golosa::golosa(vector<float> param, int N, int M, int C, int yo) : parametros(param), N(N), M(M), C(C), yo(yo) {
    assert(param.size() == PARAM_COUNT);
};

golosa::golosa(int argc, char const *argv[], int N, int M, int C, int yo) : parametros(leer_pesos(argc, argv)), N(N), M(M), C(C), yo(yo) {};


// devuelve string que comienza con los parametros de la gololsa, y termina con " "
string golosa::to_argv(){
    std::string argv;
    argv += "-f1 " + std::to_string(parametros[W_FICHAS1]) + " ";
    argv += "-f2 " + std::to_string(parametros[W_FICHAS2]) + " ";
    argv += "-d " + std::to_string(parametros[W_DISPERSION]) + " ";
    argv += "-a " + std::to_string(parametros[W_AGRESS]) + " ";
    argv += "-eh " + std::to_string(parametros[W_EXPH]) + " ";
    argv += "-ev " + std::to_string(parametros[W_EXPV]) + " ";
    argv += "-eo " + std::to_string(parametros[W_EXPO]) + " ";
    return argv;

}

std::vector<std::string> string_to_argv(string argv){
    vector< std::string > vs;
    for (uint i = 0; i < argv.size(); i++) {
        // trim initial empty spaces if there are || consequent between 'words'
        while(i < argv.size() && argv[i] == ' ') i++;
        if (i == argv.size()) break;
        // create empty string
        std::string temp;
        // fill temp with 'word'
        while(i < argv.size() && argv[i] != ' ')
            temp += argv[i];
        // when break => argv[i] == ' '
        // push temp in vs
        vs.push_back(temp);
    }
    return vs;
}


vector<float> golosa::leer_pesos(int argc, char const *argv[]){
    /* NOTE: los parámetros se cargan secuencialmente, sin flags que indiquen
    qué son. Es decir, deben estar en el mismo orden en que se definen en los
    #define de c_linea.hpp. Esto es para hacer más fácil el agregado de
    parámetros.
    */
    vector<float> pesos;
    for (size_t i = 1; i < PARAM_COUNT + 1; i++) {
        pesos.push_back(stof(argv[i]));
    }
    return pesos;
    /* El código de abajo es el viejo, que usaba flags para identificarlos */
    /*
    for (int i = 0; i < argc; i++) {
        const char* param = argv[i];
        if (strcmp(param, "-f1") == 0 || strcmp(param, "--fichas1")) {
            parametros[W_FICHAS1] = stof(argv[i+1]);
        }else if(strcmp(param, "-f2") == 0 || strcmp(param, "--fichas2")) {
            parametros[W_FICHAS1] = stof(argv[i+1]);
        }else if(strcmp(param, "-d") == 0 || strcmp(param, "--dispersion")) {
            parametros[W_DISPERSION] = stof(argv[i+1]);
        }else if(strcmp(param, "-a") == 0 || strcmp(param, "--agresividad")) {
            parametros[W_AGRESS] = stof(argv[i+1]);
        }else if(strcmp(param, "-eh") == 0 || strcmp(param, "--expansionh")) {
            parametros[W_EXPH] = stof(argv[i+1]);
        }else if(strcmp(param, "-ev") == 0 || strcmp(param, "--expansionv")) {
            parametros[W_EXPV] = stof(argv[i+1]);
        }else if(strcmp(param, "-eo") == 0 || strcmp(param, "--expansiono")) {
            parametros[W_EXPO] = stof(argv[i+1]);
        }
        // ...
    }
    */
}
