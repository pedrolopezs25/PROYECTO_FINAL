#include "nota.h"
#include <QBrush>
#include <cmath>

Nota::Nota(double x, double y, int tecla, bool seMece)
    : Objeto(x, y, 30, 30)   // llama al constructor del padre (cuadro de 30x30)
{
    this->tecla = tecla;
    this->seMece = seMece;
    xCentro = x;
    amplitud = 20;     // valores normales (dificultad facil)
    frecuencia = 1;
    tiempo = 0;

    velY = 3;          // velocidad de caida (viene de Objeto)

    // Las que se mecen son naranjas, las normales amarillas
    if (seMece) {
        setBrush(QBrush(QColor(255, 140, 0)));
    } else {
        setBrush(QBrush(Qt::yellow));
    }

    // Imagen segun la tecla: nota_a.png, nota_s.png, nota_d.png o nota_f.png
    QString letra = QString(QChar(tecla)).toLower();
    ponerImagen(":/imagenes/nota_" + letra + ".png");
}

void Nota::mover()
{
    // Siempre baja
    posY = posY + velY;

    // Si se mece: x = xCentro + amplitud * sin(frecuencia * t)
    if (seMece) {
        tiempo = tiempo + 0.1;
        posX = xCentro + amplitud * std::sin(frecuencia * tiempo);
    }

    setPos(posX, posY);
}

int Nota::getTecla()
{
    return tecla;
}

void Nota::setMeneo(double nuevaAmplitud, double nuevaFrecuencia)
{
    amplitud = nuevaAmplitud;
    frecuencia = nuevaFrecuencia;
}