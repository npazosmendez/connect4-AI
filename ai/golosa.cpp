#include "golosa.hpp"
#include "c_linea.hpp"
#include <limits>
#include <string.h>
#include <string>
#include <cmath>

using namespace std;


int golosa::jugar(c_linea juego){
    /* Dentro de las jugadas posibles, esta función elige la que lleva al juego
    al mejor tablero posible para el jugador, cuyo 'puntaje' se determina
    con la función de tal nombre, combinando features del tablero. */
    if (primera_jugada(juego) && (int)parametros[PRIMERA_JUGADA] > -1){
        return (int)parametros[PRIMERA_JUGADA];
    }
    int yo = juego.turno();
    int el = 0;
    if (yo == 1) el = 2; else el = 1;
    int res = -1;
    float puntaje_max = 0;
    for (int i = 0; i < juego.N; i++) {
        // para cada jugada posible
        if (juego.tablero()[i][juego.M-1]==0) {
            juego.jugar(yo, i);
            if (juego.gano(yo) || imbatible(juego, yo)) {
                // Gané o puedo ganar en la próxima seguro
                return i;
            }
            float temp = puntaje(juego,i, yo, el);
            if (temp > puntaje_max || res == -1) {
                res = i;
                puntaje_max = temp;
            }
            juego.desjugar(yo, i);
        }
    }
    return res;
}

float golosa::puntaje(c_linea juego, int jugada_recien, int yo, int el){
	float puntaje=0;

    ////////////////////////////////////
	/* Me guardo los features y demás */
    ////////////////////////////////////

	// uint fichas1 = juego.fichas1 ();
	// uint fichas2 = juego.fichas2 ();
	uint dispersionJ1 = dispersion (juego, yo);
	uint dispersionJ2 = dispersion (juego, el);
	uint perjudica = perjudica_rival (juego, jugada_recien) * N/2; /* 0-1 -> 0 - N/2 */
	uint expHorizontal = exp_horizontal (juego, jugada_recien) * N/2;
	uint expVertical = exp_vertical (juego, jugada_recien) * N/2;
	uint expOblicua = exp_oblicua (juego, jugada_recien) * N/2;
    vector<int> lineas_ext_yo = lineas_extensibles(juego, yo);
    vector<int> lineas_ext_el = lineas_extensibles(juego, el);

    /* EMERGENCY BUTTON !!!! */
    if (servida(juego)) {
        // Le estoy dejando una línea servida!
        // Devuelvo -infinito
        float menos_infinito = - numeric_limits<float>::max();
        return menos_infinito;
    }

    //////////////////////////////////////////////////
    /* Combino algunos features formando un puntaje */
    //////////////////////////////////////////////////

	// puntaje += fichas1 * parametros[W_FICHAS1];
	// puntaje += fichas2 * parametros[W_FICHAS2];
	puntaje += dispersionJ1 * parametros[W_DISPERSION1];
	puntaje += dispersionJ2 * parametros[W_DISPERSION2];
	puntaje += perjudica * parametros[W_AGRESS];
	puntaje += expHorizontal * parametros[W_EXPH];
	puntaje += expVertical * parametros[W_EXPVO];
	puntaje += expOblicua * parametros[W_EXPVO];
    for (int i = 0; i < C; i++) {
        // cantidad de líneas extensibles a C de longitud 'i'
        puntaje += pesos_lineas[i]*lineas_ext_yo[i]/(N*M/2); // las mías suman
        puntaje -= pesos_lineas[i]*lineas_ext_el[i]/(N*M/2); // las de él restan
    }

    ///////////////////////////////////////////////////////
    /* ¿Qué pasa si mi oponente ahora juega arriba mío ? */
    ///////////////////////////////////////////////////////
    if (juego.tablero()[jugada_recien][juego.M-1]==0) { // si es que puede jugar...
        juego.jugar(el, jugada_recien);

        /* EMERGENCY BUTTON !!!! */
        if (imbatible(juego, el)) {
            // Le estoy dejando imbatibilidad servida!
            // Devuelvo -infinito
            float menos_infinito = - numeric_limits<float>::max();
            return menos_infinito;
        }

        juego.desjugar(el, jugada_recien);
    }

    //////////////////////////////////////////////////////////////
    /* ¿Qué pasa si esta misma jugada se la dejo a mi oponente? */
    //////////////////////////////////////////////////////////////

    juego.desjugar(yo, jugada_recien); // (intercambio jugada por la de él)
    juego.paso();
    juego.jugar(el, jugada_recien);

    /* EMERGENCY BUTTON !!!! */
    if (imbatible(juego, el)) {
        // Si juega acá es imbatible!
        // Es el mejor movimiento que puedo hacer, para intentar cagarle
        // la imbatibilidad
        float infinito = numeric_limits<float>::max();
        return infinito;
    }

    vector<int> lineas_ext_el_hipoteticas = lineas_extensibles(juego, el); // gran nombre para una variable
    for (int i = 0; i < C; i++) {
        puntaje += pesos_lineas[i]*(lineas_ext_el_hipoteticas[i]-lineas_ext_el[i])/(N*M/2);
        /* Esto suma, porque se lo estoy cagando.
         Tomo la diferencia con lo que tiene en el turno anterior,
         midiendo de alguna manera qué tanto lo cagué recién. */
    }





    return puntaje;
}


/* /////////////////////////////////////// */
// FEATURES Y DEMÁS DE UN JUEGO PARTICULAR //
/* /////////////////////////////////////// */

bool golosa::primera_jugada(const c_linea &juego){
    for (int i = 0; i < juego.N; i++) {
        if (!juego.tablero()[i][0] == 0)
            return false;
    }
    return true;
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
	if(sumados==0)
		return 0;
	else
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

vector<int> golosa::lineas_extensibles(const c_linea &juego, int jugador){
    /*
    Esta función calcula la cantidad de líneas exentibles a una de C del
    color del jugador, que ya tengan 'i' fichas del jugador
    (no necesariamente inmediatamente extensibles, podría faltar
    altura de columnas).
    */
    assert(juego.M == M);
    assert(juego.N == N);
    assert(juego.C == C);
    assert(jugador == 1 || jugador == 2);
    vector<int> res(C);

    // busco líneas horizontales
    // NOTE: este está más o menos comentado. El vertical y diagonal son
    // escencialmente iguales.
    for (int fil = 0; fil < M; fil++) { // fila 'fil'
        int contador = 0; // cuenta casilleros libres o de color 'jugador' consecutivos
        int cant_completos = 0; // cuenta la cantidad de fichas propias en la línea que se está analizando actualmente
        for (int col = 0; col < N; col++) { // columna 'col'
            if (juego.tablero()[col][fil] == 0) {
                // la línea sigue, y es otro espacio libre pa llenar
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                // la línea sigue, pero este ya lo llené
                contador++;
                cant_completos++;
            }else{
                // se cortó la línea
                contador = 0;
                cant_completos = 0;
            }
            /* si vengo con buena racha y sigo extendiendo la línea más
             allá de C, me fijo si no acabo de dejar afuera una ficha mía
             que había contado, que no llena las líneas extensibles que
             contaré de ahora en más */
            if (contador > C && juego.tablero()[col-C][fil] == jugador) {
                cant_completos--;
            }
            if (contador >= C) {
                // seguí y encontré una nueva línea extendible a C, con 'cant_completos' ya puestos
                res[cant_completos]++;
            }

        }
    }
    // busco líneas verticales
    for (int col = 0; col < N; col++) { // columna 'col'
        int contador = 0;
        int cant_completos = 0;
        for (int fil = 0; fil < M; fil++) {  // fila 'fil'
            if (juego.tablero()[col][fil] == 0) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col][fil-C] == jugador) cant_completos--;
            if (contador >= C) res[cant_completos]++;
        }
    }

    // busco líneas a  45°
    for (int col_base = 0; col_base < N; col_base++) { // columna 'col_base'
        // desde la base de 'col_base' para arriba
        int contador = 0;
        int cant_completos = 0;
        for (int fil = 0; fil < M && col_base+fil<N; fil++) {
            int col = col_base+fil;
            if (juego.tablero()[col][fil] == 0) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col-C][fil-C] == jugador) cant_completos--;
            if (contador >= C) res[cant_completos]++;
        }
        // desde el tope de 'col_base' para abajo
        cant_completos = 0;
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col_base-(M-1-fil)) >= 0; fil--) {
            int col = col_base-(M-1-fil);
            if (juego.tablero()[col][fil] == 0) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col+C][fil+C] == jugador) cant_completos--;
            if (contador >= C) res[cant_completos]++;
        }
    }
    // busco en diagonales de -45°
    for (int col_base = 0; col_base < N; col_base++) {
        int contador = 0;
        int cant_completos = 0;
        // desde la base de 'col_base' para arriba
        for (int fil = 0; fil < M && col_base-fil>=0; fil++) {
            int col = col_base-fil;
            if (juego.tablero()[col][fil] == 0) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col+C][fil-C] == jugador) cant_completos--;
            if (contador >= C) res[cant_completos]++;
        }
        // desde el tope de 'col_base' para abajo
        cant_completos = 0;
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col_base+(M-1-fil)) < N; fil--) {
            int col = col_base+(M-1-fil);
            if (juego.tablero()[col][fil] == 0) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col-C][fil+C] == jugador) cant_completos--;
            if (contador >= C) res[cant_completos]++;
        }
    }

    return res;
}
bool golosa::servida(const c_linea &juego){
    /*
    Esa función verifica si el jugador que debe jugar ahora tiene una
    jugada que le da la victoria inmediata. Es decir, busca una línea
    extensible a C a la que le falte una sola ficha, pero que tal ficha
    pueda ser puesta ahora mismo (es decir, que de la altura de la columna)
    */
    int jugador = juego.turno();
    assert(juego.M == M);
    assert(juego.N == N);
    assert(juego.C == C);
    assert(jugador == 1 || jugador == 2);

    // busco líneas horizontales
    // NOTE: este está más o menos comentado. El vertical y diagonal son
    // escencialmente iguales.
    for (int fil = 0; fil < M; fil++) { // fila 'fil'
        int contador = 0; // cuenta casilleros libres llenables inmediatamente o de color 'jugador' consecutivos
        int cant_completos = 0; // cuenta la cantidad de fichas propias en la línea que se está analizando actualmente
        for (int col = 0; col < N; col++) { // columna 'col'
            if (juego.tablero()[col][fil] == 0 && juego._alturas[col] == fil) {
                // la línea sigue, y es otro espacio libre pa llenar inmediatamente
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                // la línea sigue, pero este ya lo llené
                contador++;
                cant_completos++;
            }else{
                // se cortó la línea
                contador = 0;
                cant_completos = 0;
            }
            /* si vengo con buena racha y sigo extendiendo la línea más
             allá de C, me fijo si no acabo de dejar afuera una ficha mía
             que había contado, que no llena las líneas extensibles que
             contaré de ahora en más */
            if (contador > C && juego.tablero()[col-C][fil] == jugador) {
                cant_completos--;
            }
            if (contador >= C && cant_completos == C-1) {
                return true;
            }

        }
    }
    // busco líneas verticales
    for (int col = 0; col < N; col++) { // columna 'col'
        int contador = 0;
        int cant_completos = 0;
        for (int fil = 0; fil < M; fil++) {  // fila 'fil'
            if (juego.tablero()[col][fil] == 0 && juego._alturas[col] == fil) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col][fil-C] == jugador) cant_completos--;
            if (contador >= C && cant_completos == C-1) return true;
        }
    }

    // busco líneas a  45°
    for (int col_base = 0; col_base < N; col_base++) { // columna 'col_base'
        // desde la base de 'col_base' para arriba
        int contador = 0;
        int cant_completos = 0;
        for (int fil = 0; fil < M && col_base+fil<N; fil++) {
            int col = col_base+fil;
            if (juego.tablero()[col][fil] == 0 && juego._alturas[col] == fil) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col-C][fil-C] == jugador) cant_completos--;
            if (contador >= C && cant_completos == C-1) return true;
        }
        // desde el tope de 'col_base' para abajo
        cant_completos = 0;
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col_base-(M-1-fil)) >= 0; fil--) {
            int col = col_base-(M-1-fil);
            if (juego.tablero()[col][fil] == 0 && juego._alturas[col] == fil) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col+C][fil+C] == jugador) cant_completos--;
            if (contador >= C && cant_completos == C-1) return true;
        }
    }
    // busco en diagonales de -45°
    for (int col_base = 0; col_base < N; col_base++) {
        int contador = 0;
        int cant_completos = 0;
        // desde la base de 'col_base' para arriba
        for (int fil = 0; fil < M && col_base-fil>=0; fil++) {
            int col = col_base-fil;
            if (juego.tablero()[col][fil] == 0 && juego._alturas[col] == fil) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col+C][fil-C] == jugador) cant_completos--;
            if (contador >= C && cant_completos == C-1) return true;
        }
        // desde el tope de 'col_base' para abajo
        cant_completos = 0;
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col_base+(M-1-fil)) < N; fil--) {
            int col = col_base+(M-1-fil);
            if (juego.tablero()[col][fil] == 0 && juego._alturas[col] == fil) {
                contador++;
            }else if (juego.tablero()[col][fil] == jugador){
                contador++;
                cant_completos++;
            }else{
                contador = 0;
                cant_completos = 0;
            }
            if (contador > C && juego.tablero()[col-C][fil+C] == jugador) cant_completos--;
            if (contador >= C && cant_completos == C-1) return true;
        }
    }

    return false;
}

bool golosa::imbatible(const c_linea &juego, int jugador){
    /* determina si hay una línea de C-1 extendible a ambos lados de
    forma inmediata (es decir, la altura de las columnas permite llenar
    esos casillero), por lo que el jugador ganaría en el próximo turno */

    // busco en filas
    for (int fil = 0; fil < M; fil++) {
        bool vacia_al_principio = false;
        int fichas_consec = 0;
        for (int col = 0; col < N; col++) {
            if (juego.tablero()[col][fil] == jugador) {
                // otra ficha consecutiva
                fichas_consec++;
            }else if(juego.tablero()[col][fil] == 0){
                // agujero. Si hubo C-1 fichas consecutivas, un vacío al
                // principio, y pueden llenarse los dos extremos, IMBATIBLE!
                if (fichas_consec == C-1 && vacia_al_principio && juego._alturas[col] == fil && juego._alturas[col-C] == fil){
                    return true;
                }else{
                    // No soy imbatible, reseteo el contador de consecutivas
                    fichas_consec = 0;
                    vacia_al_principio = true;
                }
            }else{
                // Ficha enemiga, empiezo de cero
                vacia_al_principio = false;
                fichas_consec = 0;
            }
        }
    }

    // busco en diagonales de 45°
    for (int col_base = 0; col_base < N; col_base++) {
        // desde la base de esa columna para arriba
        bool vacia_al_principio = false;
        int fichas_consec = 0;
        for (int fil = 0; fil < M && col_base+fil<N; fil++) {
            int col = col_base + fil;
            if (juego.tablero()[col][fil] == jugador) {
                fichas_consec++;
            }else if(juego.tablero()[col][fil] == 0){
                if (fichas_consec == C-1 && vacia_al_principio && juego._alturas[col] == fil && juego._alturas[col-C] == fil-C){
                    return true;
                }else{
                    fichas_consec = 0;
                    vacia_al_principio = true;
                }
            }else{
                vacia_al_principio = false;
                fichas_consec = 0;
            }
        }
        // desde el tope de esa columna para abajo
        vacia_al_principio = false;
        fichas_consec = 0;
        for (int fil = M-1; fil >= 0 && (col_base-(M-1-fil)) >= 0; fil--) {
            int col = col_base - (M-1-fil);
            if (juego.tablero()[col][fil] == jugador) {
                fichas_consec++;
            }else if(juego.tablero()[col][fil] == 0){
                if (fichas_consec == C-1 && vacia_al_principio && juego._alturas[col] == fil && juego._alturas[col+C] == fil+C){
                    return true;
                }else{
                    fichas_consec = 0;
                    vacia_al_principio = true;
                }
            }else{
                vacia_al_principio = false;
                fichas_consec = 0;
            }
        }
    }
    // busco en diagonales de -45°
    for (int col_base = 0; col_base < N; col_base++) {
        // desde la base de esa columna para arriba
        bool vacia_al_principio = false;
        int fichas_consec = 0;
        for (int fil = 0; fil < M && col_base-fil>=0; fil++) {
            int col = col_base - fil;
            if (juego.tablero()[col][fil] == jugador) {
                fichas_consec++;
            }else if(juego.tablero()[col][fil] == 0){
                if (fichas_consec == C-1 && vacia_al_principio && juego._alturas[col] == fil && juego._alturas[col+C] == fil-C){
                    return true;
                }else{
                    fichas_consec = 0;
                    vacia_al_principio = true;
                }
            }else{
                vacia_al_principio = false;
                fichas_consec = 0;
            }
        }
        // desde el tope de esa columna para abajo
        vacia_al_principio = false;
        fichas_consec = 0;
        for (int fil = M-1; fil >= 0 && (col_base+(M-1-fil)) < N; fil--) {
            int col = col_base + (M-1-fil);
            if (juego.tablero()[col][fil] == jugador) {
                fichas_consec++;
            }else if(juego.tablero()[col][fil] == 0){
                if (fichas_consec == C-1 && vacia_al_principio && juego._alturas[col] == fil && juego._alturas[col-C] == fil+C){
                    return true;
                }else{
                    fichas_consec = 0;
                    vacia_al_principio = true;
                }
            }else{
                vacia_al_principio = false;
                fichas_consec = 0;
            }
        }
    }
    return false;
}

/* /////////////////////////////////////// */
//   AUXILIARES DE INICIALIZACIÓN Y OTROS  //
//   (NO TIENEN QUE VER CON EL JUGADOR)    //
/* /////////////////////////////////////// */


// Constructores y sus parsers
golosa::golosa(int N, int M, int C) : parametros(PARAM_COUNT),pesos_lineas(C) ,N(N), M(M), C(C) {}

golosa::golosa(vector<float> param, int N, int M, int C) : parametros(param.begin(),param.begin()+PARAM_COUNT), pesos_lineas(param.begin()+PARAM_COUNT,param.end()),N(N), M(M), C(C) {
    // say_hello();
    assert(param.size() == cuantos_parametros(N,  M, C));
};

golosa::golosa(int argc, char const *argv[], int N, int M, int C) : parametros(leer_parametros(argc, argv, C)), pesos_lineas(leer_pesos_lineas(argc, argv, C)), N(N), M(M), C(C){
    // say_hello();
};

uint golosa::cuantos_parametros(int N, int M, int C){
    return PARAM_COUNT + C;
}


// devuelve string que comienza con los parametros de la gololsa, y termina con " "
/*string golosa::to_argv(){
    std::string argv;
    argv += "-f1 " + std::to_string(parametros[W_FICHAS1]) + " ";
    argv += "-f2 " + std::to_string(parametros[W_FICHAS2]) + " ";
    argv += "-d " + std::to_string(parametros[W_DISPERSION]) + " ";
    argv += "-a " + std::to_string(parametros[W_AGRESS]) + " ";
    argv += "-eh " + std::to_string(parametros[W_EXPH]) + " ";
    argv += "-ev " + std::to_string(parametros[W_EXPV]) + " ";
    argv += "-eo " + std::to_string(parametros[W_EXPO]) + " ";
    return argv;

}*/

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


void golosa::say_hello(){
    cout << "Acabás de crear un goloso, con:"<< endl;
    cout << "N: "<< N << endl;
    cout << "M: "<< M << endl;
    cout << "C: "<< C << endl;
    cout << "Parámetros: "<< endl;
    for (size_t i = 0; i < PARAM_COUNT; i++) {
        cout << parametros[i] << endl;
    }
    cout << "Pesos líneas: "<< endl;
    for (int i = 0; i < C; i++) {
        cout << pesos_lineas[i] << endl;
    }
}

vector<float> golosa::leer_pesos_lineas(int argc, char const *argv[], int C){
    vector<float> pesos;
    for (int i = PARAM_COUNT + 1; i < PARAM_COUNT+1 + C; i++) {
        pesos.push_back(stof(argv[i]));

    }
    return pesos;
}

vector<float> golosa::leer_parametros(int argc, char const *argv[], int C){
    /* NOTE: los parámetros se cargan secuencialmente, sin flags que indiquen
    qué son. Es decir, deben estar en el mismo orden en que se definen en los
    #define de c_linea.hpp. Esto es para hacer más fácil el agregado de
    parámetros.
    */
    vector<float> params;
    for (size_t i = 1; i < PARAM_COUNT+1; i++) {
        params.push_back(stof(argv[i]));

    }
    params[PRIMERA_JUGADA] = int(params[PRIMERA_JUGADA]);
    // assert((int)params[PRIMERA_JUGADA] >= -1 && (int)params[PRIMERA_JUGADA] < C);

    return params;
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
