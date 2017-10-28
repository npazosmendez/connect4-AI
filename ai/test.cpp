#include "c_linea.hpp"

int main(int argc, char const *argv[]) {
    int C = 4;
    int N = 10;
    int M = 6;
    int p = 100;
    c_linea juego(C,N,M,p);
    juego.jugar1(1);
    juego.jugar2(2);
    juego.jugar1(1);
    juego.jugar2(0);
    juego.jugar1(3);
    juego.jugar2(0);
    juego.jugar1(0);
    juego.jugar2(4);
    juego.jugar1(2);
    juego.jugar2(3);
    juego.jugar1(4);
    juego.jugar2(2);
    juego.jugar1(1);
    juego.jugar2(1);
    juego.mostrar();
    cout << "ganó 1? : " << juego.gano1() << endl;
    cout << "ganó 2? : " << juego.gano2() << endl;

    cout << endl;
    cout << endl;
    juego.desjugar2(1);
    juego.jugar2(5);
    juego.jugar1(2);
    juego.jugar2(3);
    juego.jugar1(2);
    juego.jugar2(2);
    juego.jugar1(6);
    juego.jugar2(6);
    juego.jugar1(5);
    juego.jugar2(4);
    juego.jugar1(4);
    juego.jugar2(4);
    juego.jugar1(3);
    juego.jugar2(6);
    juego.mostrar();
    cout << "ganó 1? : " << juego.gano1() << endl;
    cout << "ganó 2? : " << juego.gano2() << endl;

    cout << "Testing lineas_nuevas(...): "<< endl;
    cout << juego.lineas_nuevas(4, 1,1) << endl;
    return 0;
}
