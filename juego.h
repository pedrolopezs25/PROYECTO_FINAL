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
#include "trampa.h"

// Clase que maneja las REGLAS del juego.
// Tiene la escena (donde se dibuja todo), el timer y todos los objetos.
// La ventana (MainWindow) solo la muestra y le pasa las teclas.
class Juego : public QObject
{
    Q_OBJECT

public:
    Juego(QString imagenJugador, QObject *padre = nullptr);

    QGraphicsScene* getEscena();

    // Empieza un nivel que dura "segundos". Lanza una EXCEPCION si el nivel no existe.
    void iniciarNivel(int numeroNivel, bool modoDificil, int segundos);

    // La ventana nos avisa que tecla presiono o solto la persona
    void teclaPresionada(int tecla);
    void teclaSoltada(int tecla);

    // EFICIENCIA: si la ventana se minimiza, el juego se pausa (no gasta procesador)
    void pausar();
    void reanudar();

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
    void crearTrampa();
    void revisarInstrumentos();
    void revisarArepas();
    void revisarTrampas();

    // Generales
    void actualizarTexto();
    void actualizarReloj();
    void terminarNivel();
    void borrarTodo();

    QGraphicsScene *escena;
    QTimer *timer;
    QGraphicsTextItem *texto;      // puntos y fallos en pantalla
    QGraphicsTextItem *reloj;      // cuenta regresiva grande (arriba a la derecha)
    QGraphicsRectItem *linea;      // linea donde hay que atinarle a las notas

    Jugador *jugador;
    Rival *rival;

    QList<Nota*> notas;
    QList<Instrumento*> instrumentos;
    QList<Arepa*> arepas;
    QList<Trampa*> trampas;

    int nivel;
    bool dificil;
    int puntosPorAcierto;  // 10 en facil, 20 en dificil
    int contador;          // cuantas veces se ha llamado actualizar()
    int tiempoRestante;    // en segundos
    int lanzados;          // instrumentos lanzados (nivel 2)
};

#endif