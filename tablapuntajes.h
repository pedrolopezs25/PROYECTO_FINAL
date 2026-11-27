#ifndef TABLAPUNTAJES_H
#define TABLAPUNTAJES_H

#include <QString>
#include <QList>

// Un renglon de la tabla: quien jugo y cuantos puntos hizo
struct Registro
{
    QString nombre;
    int puntaje;
};

// TABLA DE PUNTAJES (capa logica).
// Guarda los mejores puntajes mientras el programa este abierto,
// SIEMPRE ordenados de mayor a menor, para que varios jugadores compitan.
class TablaPuntajes
{
public:
    TablaPuntajes();

    // Mete un puntaje en su lugar. Devuelve el puesto (1, 2, 3...)
    // o 0 si no alcanzo a entrar en la tabla.
    int agregar(QString nombre, int puntaje);

    // Arma la tabla como texto para mostrarla en pantalla.
    // "puestoResaltado" se pinta de otro color (el que acaba de jugar).
    QString comoTexto(int puestoResaltado);

private:
    QList<Registro> registros;   // ordenados de mayor a menor
    int maximo;                  // cuantos puestos tiene la tabla
};

#endif