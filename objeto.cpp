#include "objeto.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPen>

Objeto::Objeto(double x, double y, double ancho, double alto)
{
    posX = x;
    posY = y;
    velX = 0;   // por defecto no se mueve de lado
    velY = 2;   // por defecto cae despacio

    setRect(0, 0, ancho, alto);   // tamano del rectangulo
    setPos(posX, posY);           // lugar en la pantalla
}

void Objeto::mover()
{
    // Movimiento rectilineo: a la posicion le sumamos la velocidad
    posX = posX + velX;
    posY = posY + velY;
    setPos(posX, posY);
}

bool Objeto::salioDePantalla(double altoPantalla)
{
    // true si ya paso el borde de abajo
    return posY > altoPantalla;
}

void Objeto::ponerImagen(QString ruta)
{
    QPixmap imagen(ruta);

    // Si no encuentra la imagen, se queda el cuadro de color
    if (imagen.isNull()) {
        return;
    }

    // La ajustamos al tamano del rectangulo
    imagen = imagen.scaled(rect().width(), rect().height(),
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // La imagen es "hija" del objeto: se mueve junto con el
    new QGraphicsPixmapItem(imagen, this);

    // Escondemos el cuadro de color
    setPen(Qt::NoPen);
    setBrush(Qt::NoBrush);
}