#ifndef OBJETO_H
#define OBJETO_H

#include <QGraphicsRectItem>
#include <QString>
#include <QPixmap>
#include <QHash>

// Clase PADRE de todo lo que se mueve en el juego.
// Aqui va la FISICA BASICA: posicion y velocidad.
class Objeto : public QGraphicsRectItem
{
public:
    Objeto(double x, double y, double ancho, double alto);

    // Movimiento rectilineo: x = x + velX,  y = y + velY
    // Es "virtual" para que cada hijo lo cambie a su manera.
    virtual void mover();

    bool salioDePantalla(double altoPantalla) const;

    // Le pone una imagen encima al rectangulo (la saca de la cache)
    void ponerImagen(const QString &ruta);

    // PRECARGA: carga una imagen a la cache ANTES de jugar.
    // Si no la encuentra lanza una EXCEPCION (std::runtime_error).
    // "static" = se llama sin crear ningun objeto: Objeto::precargarImagen(...)
    static void precargarImagen(const QString &ruta, int ancho, int alto);

protected:
    double posX;   // posicion horizontal
    double posY;   // posicion vertical
    double velX;   // velocidad horizontal
    double velY;   // velocidad vertical

private:
    // Arma la "llave" con la que se guarda cada imagen en la cache
    static QString armarClave(const QString &ruta, int ancho, int alto);

    // CACHE DE IMAGENES (eficiencia):
    // guarda las imagenes ya cargadas para no leer el archivo cada vez.
    // QHash busca casi instantaneo (no recorre la lista), mejor que QMap.
    // "static" = hay UNA sola cache para toda la clase.
    static QHash<QString, QPixmap> imagenesCargadas;
};

#endif