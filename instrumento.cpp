#include "instrumento.h"
#include <QBrush>

Instrumento::Instrumento(double x, double y, double velX, double velY, bool rebota)
    : Objeto(x, y, 25, 25)   // llama al constructor del padre (cuadro de 25x25)
{
    this->velX = velX;       // velocidad inicial (la da el rival al lanzar)
    this->velY = velY;
    this->rebota = rebota;

    gravedad = 0.3;
    piso = 575;              // la escena mide 600 de alto
    restitucion = 0.6;

    // Los que rebotan son morados, los normales rojos
    if (rebota) {
        setBrush(QBrush(Qt::magenta));
    } else {
        setBrush(QBrush(Qt::darkRed));
    }
}

void Instrumento::mover()
{
    // PARABOLA: en x avanza igual siempre,
    // en y la gravedad le va sumando velocidad (sube, frena y cae)
    posX = posX + velX;
    velY = velY + gravedad;
    posY = posY + velY;

    // REBOTE: si toca el piso, sube otra vez pero con menos fuerza
    if (rebota && posY >= piso) {
        posY = piso;
        velY = -velY * restitucion;   // cambia de direccion y pierde fuerza
        rebota = false;               // solo rebota una vez
    }

    setPos(posX, posY);
}