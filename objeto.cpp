#include "objeto.h"
#include <QGraphicsPixmapItem>
#include <QPen>
#include <stdexcept>

// La cache empieza vacia
QHash<QString, QPixmap> Objeto::imagenesCargadas;

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

bool Objeto::salioDePantalla(double altoPantalla) const
{
    // true si ya paso el borde de abajo
    return posY > altoPantalla;
}

QString Objeto::armarClave(const QString &ruta, int ancho, int alto)
{
    // Ejemplo: ":/imagenes/caja.png 55x55"
    return ruta + " " + QString::number(ancho) + "x" + QString::number(alto);
}

void Objeto::precargarImagen(const QString &ruta, int ancho, int alto)
{
    QPixmap imagen(ruta);

    // EXCEPCION: si la imagen no carga, avisamos con un error claro
    if (imagen.isNull()) {
        throw std::runtime_error(("No se pudo cargar la imagen: " + ruta).toStdString());
    }

    // Se ajusta al tamano UNA sola vez y se guarda en la cache
    imagen = imagen.scaled(ancho, alto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imagenesCargadas.insert(armarClave(ruta, ancho, alto), imagen);
}

void Objeto::ponerImagen(const QString &ruta)
{
    QString clave = armarClave(ruta, rect().width(), rect().height());

    // EFICIENCIA: la imagen ya esta en la cache (se precargo al abrir el juego),
    // asi que crear una nota o un instrumento NO lee ningun archivo.
    // Si por alguna razon no esta, se queda el cuadro de color.
    if (!imagenesCargadas.contains(clave)) {
        return;
    }

    // La imagen es "hija" del objeto: se mueve junto con el
    new QGraphicsPixmapItem(imagenesCargadas.value(clave), this);

    // Escondemos el cuadro de color
    setPen(Qt::NoPen);
    setBrush(Qt::NoBrush);
}