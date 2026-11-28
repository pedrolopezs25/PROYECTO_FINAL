#ifndef JUEGO_H
#define JUEGO_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsSimpleTextItem>
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
    Juego(const QString &imagenJugador, QObject *padre = nullptr);

    // Carga TODAS las imagenes de los objetos UNA vez, antes de jugar.
    // Lanza una EXCEPCION si falta alguna. Se llama al abrir el programa.
    static void precargarImagenes();

    QGraphicsScene* getEscena();

    // Empieza un nivel que dura "segundos".
    // Lanza una EXCEPCION si el nivel no existe o si la duracion no es valida.
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
    void actualizarCuentaRegresiva();   // 3, 2, 1, ¡YA! antes de empezar
    void terminarNivel();
    void borrarTodo();

    QGraphicsScene *escena;
    QTimer *timer;
    QGraphicsTextItem *texto;      // puntos y fallos en pantalla
    QGraphicsTextItem *reloj;      // cuenta regresiva grande (arriba a la derecha)
    QGraphicsRectItem *linea;      // linea donde hay que atinarle a las notas
    QGraphicsSimpleTextItem *aviso;   // numero grande de la cuenta regresiva

    Jugador *jugador;
    Rival *rival;

    QList<Nota*> notas;
    QList<Instrumento*> instrumentos;
    QList<Arepa*> arepas;
    QList<Trampa*> trampas;

    int nivel;
    bool dificil;
    int multiplicador;     // 1 en facil, 2 en dificil (los puntos valen el doble)
    int intervaloLanzar;   // cada cuantos ticks lanza el rival (dificil = mas seguido)
    int maxTrampas;        // facil 2, dificil 3
    int segundosTrampa;    // cuanto dura cada trampa: facil 5, dificil 7
    int contador;          // cuantas veces se ha llamado actualizar()
    int tiempoRestante;    // en segundos
    int lanzados;          // instrumentos lanzados (nivel 2)
    int ticksCuenta;       // ticks que le faltan a la cuenta regresiva (0 = ya se juega)
    int ticksYa;           // ticks que se ve el "¡YA!" despues de la cuenta
};

#endif