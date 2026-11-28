#ifndef RIVAL_H
#define RIVAL_H

#include <QGraphicsRectItem>
#include "instrumento.h"

// El rival del NIVEL 2. Es el AGENTE INTELIGENTE.
// Lanza instrumentos y aprende de que lado falla mas el jugador.
//   1. PERCIBIR:   ve donde cayo el instrumento y si lo atraparon
//   2. APRENDER:   guarda en su memoria los fallos de cada lado
//   3. RAZONAR:    decide hacia que lado lanzar (donde mas falla)
//   4. ACTUAR:     lanza el instrumento hacia ese lado
class Rival : public QGraphicsRectItem
{
public:
    Rival(double x, double y);

    void percibir(double xCaida, bool atrapado);
    void aprender();
    int razonar();             // devuelve -1 (izquierda) o 1 (derecha)
    Instrumento* actuar();     // crea el instrumento (memoria dinamica)

private:
    double posX;
    double posY;

    // Lo que percibio la ultima vez
    double ultimaX;
    bool ultimoAtrapado;

    // Su memoria (lo que ha aprendido)
    int fallosIzquierda;
    int fallosDerecha;
};

#endif