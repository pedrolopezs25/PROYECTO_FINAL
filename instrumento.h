#ifndef INSTRUMENTO_H
#define INSTRUMENTO_H

#include "objeto.h"

// Instrumento del NIVEL 2. HEREDA de Objeto.
// Hay 3 TIPOS, cada uno vale distinto y pesa distinto:
//   0 = guacharaca (liviana, 10 puntos)
//   1 = caja       (media,   15 puntos)
//   2 = acordeon   (pesado,  20 puntos)
// FISICA:
//   - Vuela en PARABOLA por la gravedad.
//   - Choca con los BORDES de la ventana con un choque ELASTICO
//     (rebota sin perder nada de rapidez, coeficiente = 1).
//   - Algunos rebotan UNA vez en el piso con choque INELASTICO
//     (pierden fuerza, coeficiente de restitucion = 0.6).
class Instrumento : public Objeto
{
public:
    Instrumento(double x, double y, double velX, double velY, int tipo, bool rebota);

    void mover() override;   // cambia el mover() del padre

    int getPuntos() const;    // puntos que da si lo atrapan
    double getPeso() const;   // cuanto frena al jugador que lo carga

    static const int TAMANO = 55;   // todos miden 55 x 55

private:
    void rebotarEnBordes();   // choque elastico con las paredes y el techo

    int tipo;
    int puntos;
    double peso;
    double gravedad;      // cuanto aumenta velY en cada paso
    bool rebota;          // true = rebota una vez en el piso
    double piso;          // altura del piso (en pixeles)
    double restitucion;   // fuerza que conserva al rebotar en el piso (0 a 1)
};

#endif