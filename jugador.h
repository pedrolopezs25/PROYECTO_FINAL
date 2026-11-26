#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsRectItem>
#include <QString>

// El personaje que controla la persona (abajo de la pantalla).
// Tiene 2 CARACTERISTICAS que cambian como se mueve:
//   - Si come AREPA se vuelve mas RAPIDO.
//   - Si CARGA muchos instrumentos se vuelve mas LENTO.
class Jugador : public QGraphicsRectItem
{
public:
    Jugador(double x, double y, QString rutaImagen);   // la imagen la escoge el menu

    void moverIzquierda();
    void moverDerecha();

    void comerArepa();          // caracteristica 1: rapido
    void cargarInstrumento();   // caracteristica 2: lento

    void sumarPuntos(int puntos);
    void sumarFallo();

    int getPuntaje();
    int getFallos();
    double getX();

private:
    double posX;
    double posY;
    double velocidad;   // cuantos pixeles se mueve por tecla
    int carga;          // cuantos instrumentos lleva
    int puntaje;
    int fallos;
};

#endif