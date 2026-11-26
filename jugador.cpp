#include "jugador.h"
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPen>

Jugador::Jugador(double x, double y, QString rutaImagen)
{
    posX = x;
    posY = y;
    velocidad = 15;
    carga = 0;
    puntaje = 0;
    fallos = 0;

    setRect(0, 0, 90, 120);          // tamano del personaje
    setBrush(QBrush(Qt::blue));
    setPos(posX, posY);

    // Imagen del personaje (si no la encuentra, queda el cuadro de color)
    QPixmap imagen(rutaImagen);   // la que escogio en el menu
    if (!imagen.isNull()) {
        imagen = imagen.scaled(90, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        new QGraphicsPixmapItem(imagen, this);   // hija: se mueve con el
        setPen(Qt::NoPen);
        setBrush(Qt::NoBrush);
    }
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
    if (posX > 710) {
        posX = 710;                 // 800 de ancho - 90 del jugador
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