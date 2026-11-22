#ifndef JUEGO_H
#define JUEGO_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QList>
#include "jugador.h"
#include "rival.h"
#include "nota.h"
#include "instrumento.h"
#include "arepa.h"

// Clase que maneja las REGLAS del juego.
// Tiene la escena (donde se dibuja todo), el timer y todos los objetos.
// La ventana (MainWindow) solo la muestra y le pasa las teclas.
class Juego : public QObject
{
    Q_OBJECT

public:
    Juego(QObject *padre = nullptr);

    QGraphicsScene* getEscena();

    // Empieza un nivel. Lanza una EXCEPCION si el nivel no existe.
    void iniciarNivel(int numeroNivel, bool modoDificil);

    // La ventana nos avisa que tecla presiono la persona
    void teclaPresionada(int tecla);

signals:
    // Avisos para la ventana (se conectan con connect)
    void nivelTerminado(int numeroNivel, int puntaje);
    void acierto();
    void fallo();

private slots:
    void actualizar();   // el timer la llama cada 30 ms

private:
    // Nivel 1
    void crearNota();
    void revisarTecla(int tecla);
    void revisarNotasPerdidas();

    // Nivel 2
    void lanzarInstrumento();
    void crearArepa();
    void revisarInstrumentos();
    void revisarArepas();

    // Generales
    void actualizarTexto();
    void terminarNivel();
    void borrarTodo();

    QGraphicsScene *escena;
    QTimer *timer;
    QGraphicsTextItem *texto;      // puntos y tiempo en pantalla
    QGraphicsRectItem *linea;      // linea donde hay que atinarle a las notas

    Jugador *jugador;
    Rival *rival;

    QList<Nota*> notas;
    QList<Instrumento*> instrumentos;
    QList<Arepa*> arepas;

    int nivel;
    bool dificil;
    int contador;          // cuantas veces se ha llamado actualizar()
    int tiempoRestante;    // en segundos (nivel 1)
    int lanzados;          // instrumentos lanzados (nivel 2)
};

#endif