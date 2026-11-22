#ifndef NOTA_H
#define NOTA_H

#include "objeto.h"

// Nota musical del NIVEL 1. HEREDA de Objeto.
// Cae hacia abajo y algunas se mecen de lado a lado.
// FISICA: movimiento oscilatorio  x = xCentro + amplitud * sin(frecuencia * t)
class Nota : public Objeto
{
public:
    Nota(double x, double y, int tecla, bool seMece);

    void mover() override;   // cambia el mover() del padre

    int getTecla();

    // Para la dificultad: en dificil se mece mas lejos y mas rapido
    void setMeneo(double nuevaAmplitud, double nuevaFrecuencia);

private:
    int tecla;           // tecla que hay que presionar (A, S, D o F)
    bool seMece;         // true = se mueve de lado a lado
    double xCentro;      // centro del meneo
    double amplitud;     // que tan lejos se mece (en pixeles)
    double frecuencia;   // que tan rapido se mece
    double tiempo;       // t de la formula
};

#endif