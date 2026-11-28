#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>

// El personaje que controla la persona (abajo de la pantalla).
// Tiene 2 CARACTERISTICAS que cambian como se mueve:
//   - Si come AREPA se vuelve mas RAPIDO.
//   - Si CARGA muchos instrumentos se vuelve mas LENTO.
// Ademas:
//   - Se mueve SUAVE: mientras la flecha este presionada avanza un poquito en cada tick.
//   - SALTA (tiro vertical con gravedad) para pasar las trampas.
//   - Se puede quedar CONGELADO si pisa una trampa.
//   - ANIMACION: mira hacia donde camina y rebota al dar pasos.
class Jugador : public QGraphicsRectItem
{
public:
    Jugador(double x, double y, QString rutaImagen);   // la imagen la escoge el menu

    // La ventana avisa si la flecha esta presionada o se solto
    void setIzquierda(bool presionada);
    void setDerecha(bool presionada);
    void saltar();

    // Se llama en cada tick (30 ms): mueve, salta y anima
    void actualizar();

    void congelar(int ticks);   // lo deja quieto un tiempo
    bool estaCongelado();

    void comerArepa();          // caracteristica 1: rapido
    void cargarInstrumento();   // caracteristica 2: lento

    void sumarPuntos(int puntos);
    void sumarFallo();

    int getPuntaje();
    int getFallos();
    double getX();

private:
    void animar();                     // escoge la pose y el rebote
    void cambiarPose(int nuevaPose);   // cambia la imagen que se ve

    double posX;
    double posY;        // altura del piso (donde esta parado)
    double velocidad;   // pixeles que avanza en cada tick
    int carga;          // cuantos instrumentos lleva
    int puntaje;
    int fallos;

    // ----- Movimiento -----
    bool izquierda;     // true mientras la flecha izquierda este presionada
    bool derecha;       // true mientras la flecha derecha este presionada

    // ----- Salto (fisica: tiro vertical) -----
    double altura;      // que tan arriba del piso esta (0 = en el piso)
    double velSalto;    // velocidad hacia arriba (baja por la gravedad)
    double gravedad;

    // ----- Congelado -----
    int ticksCongelado; // ticks que le faltan para descongelarse

    // ----- Animacion -----
    QGraphicsPixmapItem *imagen;   // la imagen que se ve (hija del jugador)
    QPixmap poseFrente;            // quieto, mirando de frente
    QPixmap poseDerecha;           // caminando a la derecha
    QPixmap poseIzquierda;         // la de la derecha volteada como espejo
    bool tieneLado;                // true si existe la imagen de lado
    int poseActual;                // 0 = frente, 1 = derecha, 2 = izquierda
    int direccion;                 // -1 = izquierda, 1 = derecha
    int paso;                      // ritmo del rebote (0 = quieto)
};

#endif