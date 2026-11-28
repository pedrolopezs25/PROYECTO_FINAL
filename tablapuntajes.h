#ifndef TABLAPUNTAJES_H
#define TABLAPUNTAJES_H

#include <QString>
#include <QList>
#include <QSet>
#include <QHash>

// Un renglon de la tabla: quien jugo, en que dificultad, cuanto duro y cuantos puntos hizo
struct Registro
{
    QString nombre;
    bool dificil;         // false = Facil, true = Dificil
    int segundos;         // duracion de la ronda que escogio
    int puntaje;
};

// TABLA DE PUNTAJES (capa logica).
// UNA sola tabla ordenada de mayor a menor, con columna de dificultad.
// Cada jugador puede tener 2 renglones: su MEJOR puntaje en Facil
// y su MEJOR puntaje en Dificil (uno no borra al otro).
// Se GUARDA en un archivo de texto (puntajes.txt) para que no se pierda
// al cerrar el juego.
class TablaPuntajes
{
public:
    TablaPuntajes();

    // ----- Archivo -----
    // Lee el archivo al abrir el juego. Si no existe, la tabla empieza vacia.
    // Lanza EXCEPCION (std::runtime_error) si existe pero no se puede abrir.
    void cargar(const QString &rutaArchivo);

    // Escribe la tabla en el archivo, SOLO si algo cambio (eficiencia).
    // Lanza EXCEPCION (std::runtime_error) si no se puede escribir.
    void guardar();

    // ----- Nombres -----
    // Lanza EXCEPCION (std::invalid_argument) si el nombre esta vacio
    // o si tiene ";" (ese simbolo separa los datos en el archivo).
    void validarNombre(const QString &nombre) const;

    bool yaJugo(const QString &nombre) const;   // true si ya esta en la tabla

    // Mejor puntaje de ese jugador en esa dificultad (-1 si nunca ha jugado ahi)
    int mejorPuntaje(const QString &nombre, bool dificil) const;

    // ----- Puntajes -----
    // Guarda el puntaje SOLO si es su nuevo record en esa dificultad.
    // Devuelve el puesto que tiene ese record en la tabla (1, 2, 3...).
    int agregar(const QString &nombre, bool dificil, int segundos, int puntaje);

    // Arma la tabla como texto (TOP 10). El renglon de ese jugador
    // en esa dificultad sale en dorado.
    QString comoTexto(const QString &nombreResaltado, bool dificilResaltado) const;

private:
    // Mete un registro en su lugar, sin desordenar la tabla
    int insertarOrdenado(const Registro &nuevo);

    // Llave para el QHash: "erika|1" (1 = dificil, 0 = facil)
    static QString armarLlave(const QString &nombre, bool dificil);

    QList<Registro> registros;     // TODOS los records, ordenados de mayor a menor
    int maximo;                    // cuantos puestos se MUESTRAN (TOP 10)
    QString ruta;                  // donde esta el archivo puntajes.txt
    bool hayCambios;               // true si hay algo nuevo sin guardar

    // EFICIENCIA: QHash y QSet buscan casi instantaneo, sin recorrer la lista.
    QHash<QString, int> mejores;   // llave "nombre|dificultad" -> mejor puntaje
    QSet<QString> nombresUsados;   // nombres que ya jugaron (en minuscula)
};

#endif