#include "juego.h"
#include <QBrush>
#include <QFont>
#include <QPixmap>
#include <stdexcept>
#include <cstdlib>

// ===================== PARTE 1: crear y empezar =====================

Juego::Juego(QObject *padre) : QObject(padre)
{
    // Escena de 800 x 600 con imagen de fondo
    escena = new QGraphicsScene(0, 0, 800, 600, this);
    QPixmap fondo(":/imagenes/fondo.png");
    if (fondo.isNull()) {
        escena->setBackgroundBrush(QBrush(QColor(40, 20, 60)));   // color si no hay imagen
    } else {
        escena->setBackgroundBrush(QBrush(fondo.scaled(800, 600)));
    }

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
    jugador = new Jugador(370, 510);
    escena->addItem(jugador);

    rival = new Rival(370, 45);
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

// ===================== PARTE 2: nivel 1 (notas) =====================

void Juego::crearNota()
{
    // 4 carriles, uno por tecla: A, S, D, F
    int teclas[4] = { Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_F };
    int carril = std::rand() % 4;
    bool seMece = (std::rand() % 2 == 0);    // la mitad se mecen

    Nota *nota = new Nota(200 + carril * 125, 40, teclas[carril], seMece);

    // DIFICULTAD: en dificil se mecen mas lejos y mas rapido
    if (dificil) {
        nota->setMeneo(60, 2);
    }

    notas.append(nota);
    escena->addItem(nota);
}

void Juego::revisarTecla(int tecla)
{
    // Buscamos una nota de esa tecla que este cerca de la linea
    for (int i = 0; i < notas.size(); i++) {
        Nota *nota = notas[i];
        bool cerca = nota->y() > 430 && nota->y() < 520;

        if (nota->getTecla() == tecla && cerca) {
            jugador->sumarPuntos(10);
            emit acierto();

            escena->removeItem(nota);
            delete nota;                // liberamos la memoria
            notas.removeAt(i);
            return;
        }
    }
    // Si no encontro ninguna, la persona fallo
    jugador->sumarFallo();
    emit fallo();
}

void Juego::revisarNotasPerdidas()
{
    // Recorremos al reves para poder borrar sin saltarnos ninguna
    for (int i = notas.size() - 1; i >= 0; i--) {
        if (notas[i]->salioDePantalla(600)) {
            jugador->sumarFallo();
            emit fallo();

            escena->removeItem(notas[i]);
            delete notas[i];
            notas.removeAt(i);
        }
    }
}

// ===================== PARTE 3: nivel 2 (instrumentos y arepas) =====================

void Juego::lanzarInstrumento()
{
    Instrumento *instrumento = rival->actuar();   // el agente decide y lanza
    instrumentos.append(instrumento);
    escena->addItem(instrumento);
    lanzados = lanzados + 1;
}

void Juego::crearArepa()
{
    double x = 50 + std::rand() % 700;
    Arepa *arepa = new Arepa(x, 40);
    arepas.append(arepa);
    escena->addItem(arepa);
}

void Juego::revisarInstrumentos()
{
    for (int i = instrumentos.size() - 1; i >= 0; i--) {
        Instrumento *instrumento = instrumentos[i];
        bool atrapado = jugador->collidesWithItem(instrumento);
        bool perdido = instrumento->salioDePantalla(600);

        if (atrapado || perdido) {
            if (atrapado) {
                jugador->sumarPuntos(10);
                jugador->cargarInstrumento();   // pesa: se vuelve mas lento
                emit acierto();
            } else {
                jugador->sumarFallo();
                emit fallo();
            }

            // El agente PERCIBE lo que paso y APRENDE
            rival->percibir(instrumento->x(), atrapado);
            rival->aprender();

            escena->removeItem(instrumento);
            delete instrumento;
            instrumentos.removeAt(i);
        }
    }
}

void Juego::revisarArepas()
{
    for (int i = arepas.size() - 1; i >= 0; i--) {
        Arepa *arepa = arepas[i];
        bool atrapada = jugador->collidesWithItem(arepa);

        if (atrapada) {
            jugador->comerArepa();              // se vuelve mas rapido
            emit acierto();
        }
        if (atrapada || arepa->salioDePantalla(600)) {
            escena->removeItem(arepa);
            delete arepa;
            arepas.removeAt(i);
        }
    }
}

// ===================== PARTE 4: texto y final =====================

void Juego::actualizarTexto()
{
    QString mensaje = "Nivel " + QString::number(nivel)
    + "    Puntos: " + QString::number(jugador->getPuntaje())
        + "    Fallos: " + QString::number(jugador->getFallos());

    if (nivel == 1) {
        mensaje = mensaje + "    Tiempo: " + QString::number(tiempoRestante);
    } else {
        mensaje = mensaje + "    Lanzados: " + QString::number(lanzados) + "/15";
    }
    texto->setPlainText(mensaje);
}

void Juego::terminarNivel()
{
    timer->stop();
    borrarTodo();
    emit nivelTerminado(nivel, jugador->getPuntaje());
}

void Juego::borrarTodo()
{
    // Borramos todo lo que quedo en pantalla (liberar memoria)
    for (int i = 0; i < notas.size(); i++) {
        escena->removeItem(notas[i]);
        delete notas[i];
    }
    notas.clear();

    for (int i = 0; i < instrumentos.size(); i++) {
        escena->removeItem(instrumentos[i]);
        delete instrumentos[i];
    }
    instrumentos.clear();

    for (int i = 0; i < arepas.size(); i++) {
        escena->removeItem(arepas[i]);
        delete arepas[i];
    }
    arepas.clear();
}