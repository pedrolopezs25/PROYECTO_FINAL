
#ifndef INSTRUMENTO_H
#define INSTRUMENTO_H

#include "objeto.h"

// Instrumento del NIVEL 2 (acordeon, caja, guacharaca). HEREDA de Objeto.
// El rival lo lanza y vuela en PARABOLA por la gravedad.
// Algunos REBOTAN en el piso y pierden fuerza al rebotar.
class Instrumento : public Objeto
{
public:
    Instrumento(double x, double y, double velX, double velY, bool rebota);

    void mover() override;   // cambia el mover() del padre

private:
    double gravedad;      // cuanto aumenta velY en cada paso
    bool rebota;          // true = rebota una vez en el piso
    double piso;          // altura del piso (en pixeles)
    double restitucion;   // fuerza que conserva al rebotar (0 a 1)
};

#endif