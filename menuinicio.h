#ifndef MENUINICIO_H
#define MENUINICIO_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include "tablapuntajes.h"

// MENU DE INICIO (capa grafica).
// Es una ventanita que sale antes del juego para escoger
// el nombre, el personaje, la dificultad y cuanto dura cada ronda.
// Revisa el nombre con la tabla: si esta vacio o repetido, NO deja jugar.
class MenuInicio : public QDialog
{
    Q_OBJECT

public:
    MenuInicio(const TablaPuntajes *tabla, QWidget *parent = nullptr);

    QString getNombre();            // nombre que escribio el jugador
    QString getImagenPersonaje();   // ruta de la imagen del personaje escogido
    bool esDificil();               // true si escogio "Dificil"
    int getDuracion();              // segundos de cada ronda: 30, 45 o 60

private slots:
    void intentarJugar();               // boton JUGAR: valida y, si todo esta bien, cierra
    void mostrarFoto(int personaje);    // cambia la foto cuando escogen otro personaje

private:
    void ponerColores();   // estilo del menu (colores del fondo del juego)

    const TablaPuntajes *tabla;     // para revisar si el nombre ya jugo
    QLineEdit *campoNombre;         // cajita para escribir el nombre
    QComboBox *comboPersonaje;      // lista desplegable de personajes
    QComboBox *comboDificultad;     // lista desplegable de dificultad
    QComboBox *comboTiempo;         // lista desplegable de duracion
    QLabel *foto;                   // foto del personaje escogido
    QPixmap fotos[5];               // EFICIENCIA: las 5 fotos se cargan UNA vez
};

#endif