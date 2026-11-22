#include "jugador.h"
#include <QBrush>

Jugador::Jugador(double x, double y)
{
    posX = x;
    posY = y;
    velocidad = 15;
    carga = 0;
    puntaje = 0;
    fallos = 0;

    setRect(0, 0, 60, 40);          // tamano del personaje
    setBrush(QBrush(Qt::blue));
    setPos(posX, posY);
}

void Jugador::moverIzquierda()
{
    posX = posX - velocidad;
    if (posX < 0) {
        posX = 0;                   // no se sale por la izquierda
    }
    setPos(posX, posY);
}

void Jugador::moverDerecha()
{
    posX = posX + velocidad;
    if (posX > 740) {
        posX = 740;                 // 800 de ancho - 60 del jugador
    }
    setPos(posX, posY);
}

void Jugador::comerArepa()
{
    // Caracteristica 1: la arepa le da energia, se mueve mas rapido
    velocidad = velocidad + 5;
    if (velocidad > 30) {
        velocidad = 30;             // limite para que no sea exagerado
    }
}

void Jugador::cargarInstrumento()
{
    // Caracteristica 2: entre mas carga, mas lento
    carga = carga + 1;
    velocidad = velocidad - 1;
    if (velocidad < 5) {
        velocidad = 5;              // nunca se queda quieto del todo
    }
}

void Jugador::sumarPuntos(int puntos)
{
    puntaje = puntaje + puntos;
}

void Jugador::sumarFallo()
{
    fallos = fallos + 1;
}

int Jugador::getPuntaje()
{
    return puntaje;
}

int Jugador::getFallos()
{
    return fallos;
}

double Jugador::getX()
{
    return posX;
}