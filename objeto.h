#ifndef OBJETO_H
#define OBJETO_H

#include <QGraphicsRectItem>
#include <QString>

// Clase PADRE de todo lo que se mueve en el juego.
// Aqui va la FISICA BASICA: posicion y velocidad.
class Objeto : public QGraphicsRectItem
{
public:
    Objeto(double x, double y, double ancho, double alto);

    // Movimiento rectilineo: x = x + velX,  y = y + velY
    // Es "virtual" para que cada hijo lo cambie a su manera.
    virtual void mover();

    bool salioDePantalla(double altoPantalla);

    // Le pone una imagen encima al rectangulo (si la encuentra)
    void ponerImagen(QString ruta);

protected:
    double posX;   // posicion horizontal
    double posY;   // posicion vertical
    double velX;   // velocidad horizontal
    double velY;   // velocidad vertical
};

#endif