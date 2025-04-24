#include "Game.h" 
#include <tchar.h> 

using namespace sf; 

int _tmain(int argc, _TCHAR* argv[])
{
    Game* Juego;
    Juego = new Game(800, 600, "Introducción a Box2D - Ejercicio 2 - MAVII");
    Juego->Loop();

    return 0; 
}
