#include "tablapuntajes.h"

TablaPuntajes::TablaPuntajes()
{
    maximo = 10;   // solo guardamos el TOP 10
}

int TablaPuntajes::agregar(QString nombre, int puntaje)
{
    Registro nuevo;
    nuevo.nombre = nombre;
    nuevo.puntaje = puntaje;

    // EFICIENCIA: como la lista ya esta ordenada, no hay que ordenarla toda.
    // Solo buscamos el primer puesto con menos puntos y metemos el nuevo ahi.
    int posicion = 0;
    while (posicion < registros.size() && registros[posicion].puntaje >= puntaje) {
        posicion = posicion + 1;
    }

    // Si quedo despues del ultimo puesto, no entra a la tabla
    if (posicion >= maximo) {
        return 0;
    }

    registros.insert(posicion, nuevo);

    // Si la tabla se paso del maximo, sacamos el ultimo
    if (registros.size() > maximo) {
        registros.removeLast();
    }

    return posicion + 1;   // puesto empezando en 1
}

QString TablaPuntajes::comoTexto(int puestoResaltado)
{
    // Usamos una tabla HTML: QMessageBox sabe mostrarla
    QString texto = "<table border='1' cellpadding='4'>";
    texto = texto + "<tr><th>Puesto</th><th>Nombre</th><th>Puntos</th></tr>";

    for (int i = 0; i < registros.size(); i++) {
        int puesto = i + 1;

        // El que acaba de jugar sale en dorado
        if (puesto == puestoResaltado) {
            texto = texto + "<tr style='background-color:gold'>";
        } else {
            texto = texto + "<tr>";
        }

        texto = texto + "<td>" + QString::number(puesto) + "</td>"
                + "<td>" + registros[i].nombre.toHtmlEscaped() + "</td>"
                + "<td>" + QString::number(registros[i].puntaje) + "</td></tr>";
    }

    texto = texto + "</table>";
    return texto;
}