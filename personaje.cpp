#include "personaje.h"
#include <QBrush>
#include <QPen>
#include <QTransform>

Personaje::Personaje(double x, double y, const QString &rutaFrente, const QString &rutaLado)
{
    posX = x;
    posY = y;

    setRect(0, 0, ANCHO, ALTO);
    setBrush(QBrush(Qt::blue));                 // color si no hay imagen
    setPos(posX, posY);
    setTransformOriginPoint(ANCHO / 2, ALTO);   // si gira o se estira, lo hace desde los pies

    // ----- Cargamos las poses UNA sola vez (eficiencia) -----
    poseDerecha = QPixmap(rutaLado);
    tieneLado = !poseDerecha.isNull();
    if (tieneLado) {
        poseDerecha = poseDerecha.scaled(ANCHO, ALTO, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // Para la izquierda NO hace falta otra imagen: scale(-1, 1) = espejo
        poseIzquierda = poseDerecha.transformed(QTransform().scale(-1, 1));
    }

    imagen = nullptr;
    poseActual = 0;
    poseFrente = QPixmap(rutaFrente);
    if (!poseFrente.isNull()) {
        poseFrente = poseFrente.scaled(ANCHO, ALTO, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imagen = new QGraphicsPixmapItem(this);   // hija: se mueve con el
        imagen->setTransformationMode(Qt::SmoothTransformation);
        poseActual = -1;                          // para que cambiarPose la ponga
        cambiarPose(0);
        setPen(Qt::NoPen);
        setBrush(Qt::NoBrush);
    }
}

double Personaje::getX() const
{
    return posX;
}

void Personaje::mirar(int direccion)
{
    // Sin imagen de lado: en vez de voltearse, se inclina un poquito
    if (!tieneLado) {
        setRotation(direccion * 10);
        return;
    }
    if (direccion == 1) {
        cambiarPose(1);
    } else if (direccion == -1) {
        cambiarPose(2);
    } else {
        cambiarPose(0);
    }
}

void Personaje::cambiarPose(int nuevaPose)
{
    // EFICIENCIA: si ya tiene esa pose, no hacemos nada
    if (imagen == nullptr || nuevaPose == poseActual) {
        return;
    }
    poseActual = nuevaPose;

    if (nuevaPose == 1) {
        imagen->setPixmap(poseDerecha);
    } else if (nuevaPose == 2) {
        imagen->setPixmap(poseIzquierda);
    } else {
        imagen->setPixmap(poseFrente);
    }

    // Centramos la imagen y la ponemos con los pies abajo
    double ancho = imagen->pixmap().width();
    double alto = imagen->pixmap().height();
    imagen->setPos((ANCHO - ancho) / 2, ALTO - alto);
}