#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "juego.h"
#include "tablapuntajes.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// La VENTANA del juego (capa grafica).
// Muestra la escena del Juego, le pasa las teclas y pone los sonidos.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *evento) override;     // cuando se presiona una tecla
    void keyReleaseEvent(QKeyEvent *evento) override;   // cuando se suelta una tecla
    void changeEvent(QEvent *evento) override;          // minimizar / restaurar la ventana

private slots:
    void sonarAcierto();
    void sonarFallo();
    void cuandoTerminaNivel(int numeroNivel, int puntaje);

private:
    void ponerEstilo();        // decora TODOS los cuadros de aviso del juego
    void verificarRecursos();  // revisa imagenes y sonidos: lanza EXCEPCION si falta alguno
    void nuevaPartida();     // muestra el menu y arranca un juego nuevo
    void empezarNivel(int numeroNivel);   // arranca un nivel atrapando sus excepciones
    void mostrarTabla(int puntaje);   // tabla al final + jugar otra vez o salir
    void reiniciarMusica();  // vuelve a poner la cancion desde el principio

    Ui::MainWindow *ui;
    QGraphicsView *vista;   // el "lente" que muestra la escena
    Juego *juego;
    bool dificil;
    int duracion;            // segundos de cada ronda (lo escoge el menu)
    QString nombreJugador;   // el que esta jugando ahora
    TablaPuntajes tabla;     // se carga y se guarda en puntajes.txt

    // Sonidos: cada uno necesita un reproductor y una salida de audio
    QMediaPlayer *musica;
    QAudioOutput *salidaMusica;
    QMediaPlayer *sonidoAcierto;
    QAudioOutput *salidaAcierto;
    QMediaPlayer *sonidoFallo;
    QAudioOutput *salidaFallo;
};
#endif