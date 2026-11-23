#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QKeyEvent>
#include "juego.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// La VENTANA del juego (capa grafica).
// Muestra la escena del Juego y le pasa las teclas.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *evento) override;   // cuando se presiona una tecla

private slots:
    void cuandoTerminaNivel(int numeroNivel, int puntaje);

private:
    Ui::MainWindow *ui;
    QGraphicsView *vista;   // el "lente" que muestra la escena
    Juego *juego;
    bool dificil;
};
#endif