#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>

// Clase PADRE de los personajes con imagen: Jugador y Rival.
// Se encarga de lo que los dos tienen en comun:
//   - Cargar las poses UNA sola vez (frente, derecha, izquierda).
//   - Mirar hacia un lado (la izquierda es la derecha en espejo).
// Asi no repetimos el mismo codigo en las dos clases (HERENCIA).
class Personaje : public QGraphicsRectItem
{
public:
    // rutaFrente = imagen de frente, rutaLado = imagen mirando a la derecha
    Personaje(double x, double y, const QString &rutaFrente, const QString &rutaLado);

    double getX() const;

    static const int ANCHO = 100;   // un poquito mas grandes que antes (90 x 120)
    static const int ALTO = 135;

protected:
    // -1 = mira a la izquierda, 0 = de frente, 1 = mira a la derecha
    void mirar(int direccion);

    double posX;
    double posY;

private:
    void cambiarPose(int nuevaPose);   // cambia la imagen que se ve

    QGraphicsPixmapItem *imagen;   // la imagen que se ve (hija del personaje)
    QPixmap poseFrente;
    QPixmap poseDerecha;
    QPixmap poseIzquierda;         // la de la derecha volteada como espejo
    bool tieneLado;                // true si existe la imagen de lado
    int poseActual;                // 0 = frente, 1 = derecha, 2 = izquierda
};

#endif