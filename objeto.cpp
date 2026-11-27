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

// La cache empieza vacia
QMap<QString, QPixmap> Objeto::imagenesCargadas;

void Objeto::ponerImagen(QString ruta)
{
    // Clave = ruta + tamano (guardamos la imagen ya ajustada a ese tamano)
    QString clave = ruta + " " + QString::number(rect().width())
                    + "x" + QString::number(rect().height());

    // EFICIENCIA: solo la primera vez se lee el archivo y se ajusta.
    // Las siguientes veces se reutiliza la que ya esta en la cache.
    if (!imagenesCargadas.contains(clave)) {
        QPixmap imagen(ruta);

        // Si no encuentra la imagen, se queda el cuadro de color
        if (imagen.isNull()) {
            return;
        }

        imagen = imagen.scaled(rect().width(), rect().height(),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imagenesCargadas[clave] = imagen;   // la guardamos para la proxima
    }

    // La imagen es "hija" del objeto: se mueve junto con el
    new QGraphicsPixmapItem(imagenesCargadas[clave], this);

    // Escondemos el cuadro de color
    setPen(Qt::NoPen);
    setBrush(Qt::NoBrush);
}