#include "rival.h"
#include <QBrush>
#include <cstdlib>

Rival::Rival(double x, double y)
{
    posX = x;
    posY = y;
    ultimaX = 0;
    ultimoAtrapado = true;
    fallosIzquierda = 0;     // al inicio no sabe nada
    fallosDerecha = 0;

    setRect(0, 0, 60, 40);
    setBrush(QBrush(Qt::darkGreen));
    setPos(posX, posY);
}

// 1. PERCIBIR: se fija donde cayo el instrumento y si lo atraparon
void Rival::percibir(double xCaida, bool atrapado)
{
    ultimaX = xCaida;
    ultimoAtrapado = atrapado;
}

// 2. APRENDER: si el jugador fallo, lo anota en su memoria
void Rival::aprender()
{
    if (ultimoAtrapado == false) {
        if (ultimaX < 400) {                 // la pantalla mide 800, la mitad es 400
            fallosIzquierda = fallosIzquierda + 1;
        } else {
            fallosDerecha = fallosDerecha + 1;
        }
    }
}

// 3. RAZONAR: decide el lado donde el jugador falla mas
int Rival::razonar()
{
    if (fallosIzquierda > fallosDerecha) {
        return -1;                           // izquierda
    }
    if (fallosDerecha > fallosIzquierda) {
        return 1;                            // derecha
    }
    // empate: no sabe todavia, escoge al azar
    if (std::rand() % 2 == 0) {
        return -1;
    }
    return 1;
}

// 4. ACTUAR: lanza un instrumento hacia el lado que decidio
Instrumento* Rival::actuar()
{
    int lado = razonar();

    double velX = lado * (2 + std::rand() % 3);   // entre 2 y 4 hacia ese lado
    double velY = -8;                             // negativo = hacia arriba
    bool rebota = (std::rand() % 3 == 0);         // 1 de cada 3 rebota

    return new Instrumento(posX + 20, posY + 40, velX, velY, rebota);
}