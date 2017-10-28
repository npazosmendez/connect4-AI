#include "c_linea.hpp"
#define ASSERT

c_linea::c_linea(uint C, uint N, uint M, uint p) : C(C), N(N), M(M), yo(1), _p1(p), _p2(p), _tablero(N,vector<int>(M,0)), _alturas(N,0), _turno(1) {
    // por defecto, 'yo' = jug. 1
}

c_linea::c_linea(uint C, uint N, uint M, uint p, uint yo_v) : C(C), N(N), M(M), yo(yo_v), _p1(p), _p2(p), _tablero(N,vector<int>(M,0)), _alturas(N,0), _turno(1) {
    #ifdef ASSERT
    assert(yo==1 || yo==2);
    #endif
}

void c_linea::jugar(int jugador, int columna){
    #ifdef ASSERT
    assert(columna < N);
    assert(jugador==1 || jugador==2);
    assert(_alturas[columna] < M);
    assert(_turno==(uint)jugador);
    assert(!termino());
    #endif
    if (jugador==1){
        _p1--;
        _turno = 2;
    }else{
        _p2--;
        _turno = 1;
    }
    _tablero[columna][_alturas[columna]] = jugador;
    _alturas[columna]++;
}

void c_linea::desjugar(int jugador, int columna){
    #ifdef ASSERT
    assert(columna < N);
    assert(jugador==1 || jugador==2);
    assert(_alturas[columna] > 0);
    assert(_tablero[columna][_alturas[columna]-1]==jugador);
    #endif
    if (jugador==1){
        assert(_turno==2);
        _p1++;
        _turno = 1;
    }else{
        assert(_turno==1);
        _p2++;
        _turno = 2;
    }
    _alturas[columna]--;
    _tablero[columna][_alturas[columna]] = 0;
}

bool c_linea::termino(){
    // si alguien ya ganó o no hay fichas, terminó
    if(gano1() || gano2() || _p2 == 0)
        return true;
    // si están todas las columnas llenas, terminó
    for (int i = 0; i < N; i++) {
        if (_alturas[i] < M)
            return false;
    }
    return true;
}

bool c_linea::gano(int jugador){
    assert(jugador == 1 || jugador ==2);
    // busco en filas
    for (int fil = 0; fil < M; fil++) {
        int contador = 0;
        for (int col = 0; col < N; col++) {
            contador = (_tablero[col][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    // busco en columnas
    for (int col = 0; col < N; col++) {
        int contador = 0;
        for (int fil = 0; fil < M; fil++) {
            contador = (_tablero[col][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    // busco en diagonales de 45°
    for (int col = 0; col < N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < M && col+fil<N; fil++) {
            contador = (_tablero[col+fil][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col-(M-1-fil)) >= 0; fil--) {
            contador = (_tablero[col-(M-1-fil)][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    // busco en diagonales de -45°
    for (int col = 0; col < N; col++) {
        // desde la base de esa columna para arriba
        int contador = 0;
        for (int fil = 0; fil < M && col-fil>=0; fil++) {
            contador = (_tablero[col-fil][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
        // desde el tope de esa columna para abajo
        contador = 0;
        for (int fil = M-1; fil >= 0 && (col+(M-1-fil)) < N; fil--) {
            contador = (_tablero[col+(M-1-fil)][fil] == jugador) ? (contador+1) : 0;
            if (contador == C)
                return true;
        }
    }
    return false;
}

uint c_linea::contar_lineas(int contador_der, int contador_izq, int largo){
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

uint c_linea::lineas_nuevas(int largo, int columna, int jugador){
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
    assert(_alturas[columna] > 0);
    assert(_tablero[columna][_alturas[columna]-1]==jugador);
    #endif

    uint res = 0;

    // busco en columna (para abajo nada más)
    int contador = 0;
    int fila = _alturas[columna]-1;
    while(fila >= 0 && _tablero[columna][fila]==jugador && contador < largo){
        contador++;
        fila--;
    }
    if (contador == largo) res++; // encontré una
    // cout << "En columna: " << res << endl;

    // busco en fila (hacia ambos lados)
    // cuento a derecha
    int contador_der = 0;
    int col = columna+1;
    while(col < N && _tablero[col][_alturas[columna]-1]==jugador){
        contador_der++;
        col++;
    }
    // cuento a izquierda
    int contador_izq = 0;
    col = columna-1;
    while(col >= 0 && _tablero[col][_alturas[columna]-1]==jugador){
        contador_izq++;
        col--;
    }
    res += contar_lineas(contador_der, contador_izq, largo);
    // cout << "En fila: " << res << endl;

    // busco a 45° (hacia ambos lados)
    // cuento a derecha (hacia arriba)
    contador_der = 0;
    col = columna+1;
    fila = _alturas[columna]-1 +1;
    while(col < N && fila < M && _tablero[col][fila]==jugador){
        contador_der++;
        col++;
        fila++;
    }
    // cuento a izquierda (hacia abajo)
    contador_izq = 0;
    col = columna-1;
    fila = _alturas[columna]-1 -1;
    while(col >= 0 && fila >= 0 && _tablero[col][fila]==jugador){
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
    fila = _alturas[columna]-1 -1;
    while(col < N && fila >= 0 && _tablero[col][fila]==jugador){
        contador_der++;
        col++;
        fila--;
    }
    // cuento a izquierda (hacia arriba)
    contador_izq = 0;
    col = columna-1;
    fila = _alturas[columna]-1 +1;
    while(col >= 0 && fila < M && _tablero[col][fila]==jugador){
        contador_izq++;
        col--;
        fila++;
    }
    res += contar_lineas(contador_der, contador_izq, largo);
    // cout << "En -45°: " << res << endl;

    return res;
}

uint c_linea::exp_horizontal(int col){
	int fila=_alturas[col]-1;
	if (col-1>=0&&col+1<N)
		return (_tablero[col-1][fila]==_tablero[col][fila]) || (_tablero[col+1][fila]==_tablero[col][fila]);
	if (col-1<0)
		return (_tablero[col+1][fila]==_tablero[col][fila]);
	if (col+1>=N)
		return (_tablero[col-1][fila]==_tablero[col][fila]);
	return 0;
}

uint c_linea::exp_vertical(int col){//no puedo colocar una ficha debajo de otra, asi que solo importa ver hacia abajo
	int fila=_alturas[col]-1;
	if (fila-1<0)
		return false;
	else 
		return (_tablero[col][fila]==_tablero[col][fila-1]);
}

uint c_linea::exp_oblicua(int col){
	int fila=_alturas[col]-1;
	bool hay=false;
	if (col-1>=0&&fila+1<M)
		hay =hay || (_tablero[col-1][fila+1]==_tablero[col][fila]);
	if (col-1>=0&&fila-1>=0)
		hay =hay || (_tablero[col-1][fila-1]==_tablero[col][fila]);
	if (col+1<N&&fila+1<M)
		hay =hay || (_tablero[col+1][fila+1]==_tablero[col][fila]);
	if (col+1<N&&fila-1>=0)
		hay =hay || (_tablero[col+1][fila-1]==_tablero[col][fila]);
	return hay;
}		

uint c_linea::perjudica_rival(int col){
	int fila=_alturas[col]-1;
	bool hayAdyEnemigo=false;
	bool hayAdyMio=false;
	if (col-1>=0&&fila+1<M)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col-1][fila+1]!=_tablero[col][fila])&&_tablero[col-1][fila+1]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col-1][fila+1]==_tablero[col][fila]);
	}	
	if (col-1>=0&&fila-1>=0)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col-1][fila-1]!=_tablero[col][fila])&&_tablero[col-1][fila-1]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col-1][fila-1]==_tablero[col][fila]);
	}	
	if (col+1<N&&fila+1<M)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col+1][fila+1]!=_tablero[col][fila])&&_tablero[col+1][fila+1]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col+1][fila+1]==_tablero[col][fila]);
	}
	if (col+1<N&&fila-1>=0)
	{	
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col+1][fila-1]!=_tablero[col][fila])&&_tablero[col+1][fila-1]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col+1][fila-1]==_tablero[col][fila]);
	}
	if (col+1<N)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col+1][fila]!=_tablero[col][fila])&&_tablero[col+1][fila]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col+1][fila]==_tablero[col][fila]);
	}
	if (col-1>=0)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col-1][fila]!=_tablero[col][fila])&&_tablero[col-1][fila]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col-1][fila]==_tablero[col][fila]);
	}
	if (fila+1<N)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col][fila+1]!=_tablero[col][fila])&&_tablero[col][fila+1]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col][fila+1]==_tablero[col][fila]);
	}
	if (fila-1>=0)
	{
		hayAdyEnemigo = hayAdyEnemigo || ((_tablero[col][fila-1]!=_tablero[col][fila])&&_tablero[col][fila-1]!=0);
		hayAdyMio = hayAdyMio || (_tablero[col][fila-1]==_tablero[col][fila]);
	}
	return (hayAdyEnemigo&&!hayAdyMio);	
} 

uint c_linea::dispersion(int jugador){
	int distFila[M];
	for(int i=0;i<M;i++)//inicializo en 0
		distFila[i]=0;
	for(int f=0;f<M;f++){//para cada fila
		int pos1=N;
		int pos2=-1;
		bool hay=false;
		for(int c=0;c<N;c++){//recorro las columnas
			if(_tablero[c][f]==jugador){
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

void c_linea::mostrar(){
    for (int fil = M-1; fil >= 0; fil--){
        for (int col = 0; col < N; col++){
            cout << _tablero[col][fil] << " ";
        }
        cout << endl;
    }
}
