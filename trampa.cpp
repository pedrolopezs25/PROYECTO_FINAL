#include "trampa.h"
#include <QBrush>

Trampa::Trampa(double x, double y)
    : Objeto(x, y, 50, 25)   // cuadro de 50 x 25 en el piso
{
    velX = 0;              // la trampa no se mueve
    velY = 0;
    vida = 33 * 7;         // dura 7 segundos (33 ticks = 1 segundo)
    congelamiento = 100;   // congela 3 segundos (100 x 30 ms)

    setBrush(QBrush(QColor(150, 0, 0)));   // rojo oscuro si no hay imagen
    ponerImagen(":/imagenes/trampa.png");
}

void Trampa::mover()
{
    // No cambia de lugar, solo se le va acabando el tiempo
    vida = vida - 1;

    // El ultimo segundo parpadea para avisar que se va a ir
    if (vida < 33) {
        setVisible((vida / 4) % 2 == 0);
    }
}

bool Trampa::seAcabo()
{
    return vida <= 0;
}

int Trampa::getCongelamiento()
{
    return congelamiento;
}