#ifndef OBJETO_H
#define OBJETO_H

#include <QGraphicsRectItem>
#include <QString>
#include <QPixmap>
#include <QMap>

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

private:
    // CACHE DE IMAGENES (eficiencia):
    // guarda las imagenes que ya se cargaron, para no leer el archivo cada vez.
    // "static" = hay UNA sola cache para toda la clase, compartida por todos los objetos.
    static QMap<QString, QPixmap> imagenesCargadas;
};

#endif