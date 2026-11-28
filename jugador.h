#ifndef JUGADOR_H
#define JUGADOR_H

#include "personaje.h"

// El personaje que controla la persona (abajo de la pantalla). HEREDA de Personaje.
// Tiene 2 CARACTERISTICAS que cambian como se mueve:
//   - Si come AREPA se vuelve mas RAPIDO.
//   - Si CARGA instrumentos se vuelve mas LENTO (depende del peso del instrumento).
// Ademas:
//   - Se mueve SUAVE: mientras la flecha este presionada avanza un poquito en cada tick.
//   - SALTA (tiro vertical con gravedad) para pasar las trampas.
//   - Se puede quedar CONGELADO si pisa una trampa.
//   - ANIMACION: mira hacia donde camina, rebota al dar pasos
//     y CELEBRA (se estira y da un brinquito) cuando acierta.
class Jugador : public Personaje
{
public:
    Jugador(double x, double y, const QString &rutaImagen);   // la imagen la escoge el menu

    // La ventana avisa si la flecha esta presionada o se solto
    void setIzquierda(bool presionada);
    void setDerecha(bool presionada);
    void saltar();

    // Se llama en cada tick (30 ms): mueve, salta y anima
    void actualizar();

    void celebrar();            // brinquito cuando acierta
    void congelar(int ticks);   // lo deja quieto un tiempo
    bool estaCongelado() const;
    bool estaEnElPiso() const;  // false mientras esta saltando

    void comerArepa();                  // caracteristica 1: rapido
    void cargarInstrumento(double peso);// caracteristica 2: lento

    void sumarPuntos(int puntos);
    void sumarFallo();

    int getPuntaje() const;
    int getFallos() const;

private:
    void animar();   // escoge la pose, el rebote y la celebracion

    double velocidad;   // pixeles que avanza en cada tick
    int carga;          // cuantos instrumentos lleva
    int puntaje;
    int fallos;

    // ----- Movimiento -----
    bool izquierda;     // true mientras la flecha izquierda este presionada
    bool derecha;       // true mientras la flecha derecha este presionada
    int direccion;      // -1 = izquierda, 1 = derecha
    int paso;           // ritmo del rebote (0 = quieto)

    // ----- Salto (fisica: tiro vertical) -----
    double altura;      // que tan arriba del piso esta (0 = en el piso)
    double velSalto;    // velocidad hacia arriba (baja por la gravedad)
    double gravedad;

    // ----- Congelado y celebracion -----
    int ticksCongelado; // ticks que le faltan para descongelarse
    int ticksCelebrando;// ticks que le faltan a la celebracion
};

#endif