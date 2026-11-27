#ifndef MENUINICIO_H
#define MENUINICIO_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QString>

// MENU DE INICIO (capa grafica).
// Es una ventanita que sale antes del juego para escoger
// el nombre, el personaje y la dificultad.
class MenuInicio : public QDialog
{
    Q_OBJECT

public:
    MenuInicio(QWidget *parent = nullptr);

    QString getNombre();            // nombre que escribio el jugador
    QString getImagenPersonaje();   // ruta de la imagen del personaje escogido
    bool esDificil();               // true si escogio "Dificil"

private:
    QLineEdit *campoNombre;         // cajita para escribir el nombre
    QComboBox *comboPersonaje;      // lista desplegable de personajes
    QComboBox *comboDificultad;     // lista desplegable de dificultad
};

#endif