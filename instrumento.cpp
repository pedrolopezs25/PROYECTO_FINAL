#include "instrumento.h"
#include <QBrush>

Instrumento::Instrumento(double x, double y, double velX, double velY, int tipo, bool rebota)
    : Objeto(x, y, TAMANO, TAMANO)   // llama al constructor del padre
{
    this->velX = velX;       // velocidad inicial (la da el rival al lanzar)
    this->velY = velY;
    this->tipo = tipo;
    this->rebota = rebota;

    gravedad = 0.3;
    piso = 600 - TAMANO;     // 600 de alto - lo que mide el instrumento
    restitucion = 0.6;

    // Color por si no hay imagen: los que rebotan morados, los demas rojos
    if (rebota) {
        setBrush(QBrush(Qt::magenta));
    } else {
        setBrush(QBrush(Qt::darkRed));
    }

    // Cada tipo vale y pesa distinto
    if (tipo == 0) {
        puntos = 10;
        peso = 0.2;
        ponerImagen(":/imagenes/guacharaca.png");
    }
    else if (tipo == 1) {
        puntos = 15;
        peso = 0.4;
        ponerImagen(":/imagenes/caja.png");
    }
    else {
        puntos = 20;
        peso = 0.8;
        ponerImagen(":/imagenes/acordeon.png");
    }
}

void Instrumento::mover()
{
    // PARABOLA: en x avanza igual siempre,
    // en y la gravedad le va sumando velocidad
    posX = posX + velX;
    velY = velY + gravedad;
    posY = posY + velY;

    rebotarEnBordes();

    // REBOTE EN EL PISO (inelastico): sube otra vez pero con menos fuerza
    if (rebota && posY >= piso) {
        posY = piso;
        velY = -velY * restitucion;   // cambia de direccion y pierde fuerza
        rebota = false;               // solo rebota una vez
    }

    setPos(posX, posY);
}

void Instrumento::rebotarEnBordes()
{
    // CHOQUE ELASTICO: la velocidad cambia de signo pero NO pierde rapidez.
    // Pared izquierda
    if (posX < 0) {
        posX = 0;
        velX = -velX;
    }
    // Pared derecha (800 de ancho - lo que mide el instrumento)
    if (posX > 800 - TAMANO) {
        posX = 800 - TAMANO;
        velX = -velX;
    }
    // Techo
    if (posY < 0) {
        posY = 0;
        velY = -velY;
    }
}

int Instrumento::getPuntos() const
{
    return puntos;
}

double Instrumento::getPeso() const
{
    return peso;
}