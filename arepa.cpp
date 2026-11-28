#include "arepa.h"
#include <QBrush>

Arepa::Arepa(double x, double y)
    : Objeto(x, y, 30, 20)   // llama al constructor del padre (30 de ancho, 20 de alto)
{
    velX = 0;          // cae derecho
    velY = 0;          // empieza quieta
    gravedad = 0.3;
    friccion = 0.1;    // con estos valores la velocidad limite es 0.3 / 0.1 = 3

    setBrush(QBrush(QColor(240, 210, 140)));   // color de arepa
    ponerImagen(":/imagenes/arepa.png");
}

void Arepa::mover()
{
    // La gravedad la acelera y el aire la frena
    velY = velY + gravedad - friccion * velY;

    posY = posY + velY;
    setPos(posX, posY);
}