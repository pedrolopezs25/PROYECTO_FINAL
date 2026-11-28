#ifndef RIVAL_H
#define RIVAL_H

#include "personaje.h"
#include "instrumento.h"

// El rival del NIVEL 2. Es el AGENTE INTELIGENTE. HEREDA de Personaje.
// Lanza instrumentos en CUALQUIER direccion y aprende en que ZONA falla mas
// el jugador (izquierda, centro o derecha):
//   1. PERCIBIR:   ve donde cayo el instrumento y si lo atraparon
//   2. APRENDER:   guarda en su memoria los fallos de cada zona
//   3. RAZONAR:    decide a que zona apuntar (donde mas falla)
//   4. ACTUAR:     calcula el angulo, se voltea hacia alla y lanza
class Rival : public Personaje
{
public:
    Rival(double x, double y);

    void percibir(double xCaida, bool atrapado);
    void aprender();
    int razonar();                          // devuelve la zona: 0, 1 o 2
    Instrumento* actuar(bool dificil);      // crea el instrumento (memoria dinamica)

    void actualizar();   // despues de lanzar, vuelve a mirar de frente

private:
    // Lo que percibio la ultima vez
    double ultimaX;
    bool ultimoAtrapado;

    // Su memoria (lo que ha aprendido): fallos por zona
    // EFICIENCIA: un arreglo fijo de 3 enteros, no hace falta una lista
    int fallos[3];          // [0] izquierda, [1] centro, [2] derecha

    int ticksMirando;       // cuanto tiempo se queda volteado despues de lanzar
};

#endif