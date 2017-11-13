# Estructura del código

* bin/ : contiene los binarios de los jugadores y los entrenadores, si están compilados.
* ai/ : contiene los fuentes de los jugadores (minimax y heurístico), y de los entrenadores en subdirectorios.
* mediciones/ : fuentes de todas las experimentaciones.


# Compilación

Para compilar a los jugadores puede hacerse "make" desde el directorio raiz (aquí mismo), y los ejecutables quedarán en bin/.
Para cosas específicas, cada directorio contiene su respectivo Makefile, y basta con hacer "make" en ellos.


# Ejecución del la golosa
Como se indica en el informe, el jugador goloso toma 6+C parámetros. Los parámetros se pasan como argumentos, y el ejecutable es bin/golosa.

Los órdenes de los parámetros son los siguientes:

* Primera jugada
* Dispersión propia
* Dispersión oponente
* Perjuicio al rival
* Expansión horizontal
* Expansión vertical y oblicua
* Líneas extensibles de 0 fichas
* Líneas extensibles de 1 ficha
* ...
* Líneas extensibles de C-1 fichas

Por ejemplo, desde aquí con los mejores parámetros que hallamos para el 4 en línea:

* bin/golosa 3 -21 1 17 23 0 5 32 64 97


# Ejecución de los entrenadores

Los binarios de los entrenadores se encuentran en bin/ una vez compilados. Si se ejecutan sin parámetros puede verse un 'help' que indica cómo se usan. Por salida estándar dan la información necesaria.
