#ifndef AREPA_H
#define AREPA_H

#include "objeto.h"

// Arepa que cae del cielo. HEREDA de Objeto.
// Si el jugador la atrapa, se vuelve mas rapido.
// FISICA: caida con resistencia del aire.
//   velY = velY + gravedad - friccion * velY
// Al principio acelera, pero el aire la frena hasta
// que cae a una velocidad constante (velocidad limite).
class Arepa : public Objeto
{
public:
    Arepa(double x, double y);

    void mover() override;   // cambia el mover() del padre

private:
    double gravedad;   // lo que la jala hacia abajo
    double friccion;   // lo que la frena el aire (0 a 1)
};

#endif