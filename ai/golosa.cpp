#include "golosa.hpp"
#include "c_linea.hpp"
#include "limits.h"
#include <string.h>

using namespace std;

// Constructores y sus parsers
golosa::golosa(pesos_t pesos, int N, int M, int C, int yo) : _pesos(pesos), N(N), M(M), C(C), yo(yo) {};

golosa::golosa(int argc, char const *argv[], int N, int M, int C, int yo) : _pesos(leer_pesos(argc, argv)), N(N), M(M), C(C), yo(yo) {};

golosa::pesos_t golosa::leer_pesos(int argc, char const *argv[]){
    // TODO: no tira errores. Lee lo que puede y lo demás queda por defecto. Solo falla si falla es stof().
    pesos_t pesos;
    for (int i = 0; i < argc; i++) {
        const char* param = argv[i];
        if (strcmp(param, "-f1") == 0 || strcmp(param, "--fichas1")) {
            pesos.fichas1 = stof(argv[i+2]);
        }else if(strcmp(param, "-f2") == 0 || strcmp(param, "--fichas2")) {
            pesos.fichas2 = stof(argv[i+1]);
        }else if(strcmp(param, "-hh") == 0 || strcmp(param, "--alturas")) {
            for (int h = 0; h < N; h++)
                pesos.alturas.push_back(stof(argv[i+1+h]));
        }else if(strcmp(param, "-d") == 0 || strcmp(param, "--dispersion")) {
            pesos.dispersion = stof(argv[i+1]);
        }else if(strcmp(param, "-a") == 0 || strcmp(param, "--agresividad")) {
            pesos.agresividad = stof(argv[i+1]);
        }else if(strcmp(param, "-eh") == 0 || strcmp(param, "--expansionh")) {
            pesos.expansion_h = stof(argv[i+1]);
        }else if(strcmp(param, "-ev") == 0 || strcmp(param, "--expansionv")) {
            pesos.expansion_v = stof(argv[i+1]);
        }else if(strcmp(param, "-eo") == 0 || strcmp(param, "--expansiono")) {
            pesos.expansion_o = stof(argv[i+1]);
        }
        // ...
    }
    return pesos;
}

void golosa::print_help(){
    /* Iba a hacer esto para indicar cómo llenar los pesos por los argumentos,
    pero el stdout de esto lo va a usar el python casi siempre. Queda en
    stany-by.
    */
}

// Métodos públicos

int golosa::jugar(c_linea juego){
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

// Métodos auxiliares para calcular jugada

float golosa::puntaje(c_linea &juego, int jugada_recien){
    float res = 0;

    // TODO

    return res;
}


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

uint golosa::fila_media(const c_linea &juego, int jugador){
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
        sumatoria_ponderada += (i+1) * cant_por_fila[i];
    }
    return sumatoria_ponderada/fichas_usadas;
}
uint golosa::columna_media(const c_linea &juego, int jugador){
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
        sumatoria_ponderada += (i+1) * cant_por_columna[i];
    }
    return sumatoria_ponderada/fichas_usadas;
}


//////////////////////////////////////////////////////////////////////////////
// FUNCIONES VIEJAS. VERIFICAR QUÉ SE TIRA, QUÉ SE RECICLA, QUÉ SE PRIVATIZA, ETC.
//////////////////////////////////////////////////////////////////////////////
/*
int tira_maxima(c_linea &juego, int yo);

int puntaje(pesos_t pesos, c_linea &juego){
    // Si terminó el juego, el puntaje es extremo
    if (juego.termino()){
        if (juego.gane())
            return INT_MAX;
        else if(juego.perdi())
            return INT_MIN;
        else
            return 0; // Empate. TODO: ver cómo manejar este caso
    }

    int res = 0;

    return res;
}


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

*/
