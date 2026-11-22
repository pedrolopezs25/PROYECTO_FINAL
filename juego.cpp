#include "juego.h"
#include <QBrush>
#include <QFont>
#include <stdexcept>
#include <cstdlib>

// ===================== PARTE 1: crear y empezar =====================

Juego::Juego(QObject *padre) : QObject(padre)
{
    // Escena de 800 x 600 con fondo oscuro
    escena = new QGraphicsScene(0, 0, 800, 600, this);
    escena->setBackgroundBrush(QBrush(QColor(40, 20, 60)));

    // Texto de arriba (puntos y tiempo)
    texto = new QGraphicsTextItem();
    texto->setDefaultTextColor(Qt::white);
    texto->setFont(QFont("Arial", 14));
    texto->setPos(10, 10);
    escena->addItem(texto);

    // Linea donde hay que presionar la tecla (nivel 1)
    linea = new QGraphicsRectItem(0, 0, 800, 4);
    linea->setBrush(QBrush(Qt::white));
    linea->setPos(0, 480);
    escena->addItem(linea);

    // Personajes (se crean UNA vez con new = memoria dinamica)
    jugador = new Jugador(370, 550);
    escena->addItem(jugador);

    rival = new Rival(370, 20);
    escena->addItem(rival);

    // Timer: cada 30 ms llama a actualizar()
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Juego::actualizar);

    nivel = 0;
    dificil = false;
    contador = 0;
    tiempoRestante = 0;
    lanzados = 0;
}

QGraphicsScene* Juego::getEscena()
{
    return escena;
}

void Juego::iniciarNivel(int numeroNivel, bool modoDificil)
{
    // EXCEPCION: solo existen los niveles 1 y 2
    if (numeroNivel != 1 && numeroNivel != 2) {
        throw std::invalid_argument("Ese nivel no existe. Solo hay nivel 1 y 2.");
    }

    nivel = numeroNivel;
    dificil = modoDificil;
    contador = 0;
    tiempoRestante = 45;     // el nivel 1 dura 45 segundos
    lanzados = 0;

    // En el nivel 1 se ve la linea; en el nivel 2 se ve el rival
    linea->setVisible(nivel == 1);
    rival->setVisible(nivel == 2);

    actualizarTexto();
    timer->start(30);
}

void Juego::teclaPresionada(int tecla)
{
    if (nivel == 1) {
        revisarTecla(tecla);
    }
    else if (nivel == 2) {
        if (tecla == Qt::Key_Left) {
            jugador->moverIzquierda();
        }
        else if (tecla == Qt::Key_Right) {
            jugador->moverDerecha();
        }
    }
}

void Juego::actualizar()
{
    contador = contador + 1;

    if (nivel == 1) {
        // Cada segundo (33 x 30 ms) baja el tiempo y sale una nota
        if (contador % 33 == 0) {
            tiempoRestante = tiempoRestante - 1;
            crearNota();
        }
        for (int i = 0; i < notas.size(); i++) {
            notas[i]->mover();      // polimorfismo: cada nota usa SU mover()
        }
        revisarNotasPerdidas();

        if (tiempoRestante <= 0) {
            terminarNivel();
            return;
        }
    }
    else {
        // Cada 2 segundos el rival lanza (hasta 15 instrumentos)
        if (contador % 66 == 0 && lanzados < 15) {
            lanzarInstrumento();
        }
        // Cada 6 segundos cae una arepa
        if (contador % 200 == 0) {
            crearArepa();
        }
        for (int i = 0; i < instrumentos.size(); i++) {
            instrumentos[i]->mover();
        }
        for (int i = 0; i < arepas.size(); i++) {
            arepas[i]->mover();
        }
        revisarInstrumentos();
        revisarArepas();

        // Se acaba cuando ya lanzo los 15 y no queda ninguno en el aire
        if (lanzados == 15 && instrumentos.isEmpty()) {
            terminarNivel();
            return;
        }
    }

    actualizarTexto();
}