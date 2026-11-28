#ifndef TRAMPA_H
#define TRAMPA_H

#include "objeto.h"

// TRAMPA del nivel 2: aparece en el piso y se queda quieta un rato.
// Si el jugador la PISA, la trampa desaparece y el jugador queda
// CONGELADO 3 segundos. Para pasarla hay que SALTARLA (barra espaciadora).
class Trampa : public Objeto
{
public:
    Trampa(double x, double y);

    void mover() override;   // no se mueve: solo cuenta su tiempo y parpadea
    bool seAcabo();          // true cuando ya duro todo su tiempo
    int getCongelamiento();  // cuantos ticks congela al jugador

private:
    int vida;            // ticks que le quedan en pantalla
    int congelamiento;   // ticks que congela (100 ticks = 3 segundos)
};

#endif