#include "juego.h"
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPixmap>
#include <stdexcept>
#include <cstdlib>
#include <cmath>

// ===================== PARTE 1: crear y empezar =====================

Juego::Juego(QString imagenJugador, QObject *padre) : QObject(padre)
{
    // Escena de 800 x 600 con imagen de fondo
    escena = new QGraphicsScene(0, 0, 800, 600, this);

    // EFICIENCIA: como casi todo se mueve en cada tick, le decimos a Qt
    // que no arme un indice de posiciones (se tendria que rehacer cada 30 ms)
    escena->setItemIndexMethod(QGraphicsScene::NoIndex);

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

    // Reloj de cuenta regresiva (grande, arriba a la derecha)
    reloj = new QGraphicsTextItem();
    reloj->setDefaultTextColor(Qt::white);
    reloj->setFont(QFont("Arial", 26, QFont::Bold));
    reloj->setPos(690, 0);
    escena->addItem(reloj);

    // Linea donde hay que presionar la tecla (nivel 1)
    // Amarilla con borde negro y mas gruesa (10 px) para que se vea en cualquier fondo
    linea = new QGraphicsRectItem(0, 0, 800, 10);
    linea->setBrush(QBrush(QColor(255, 210, 0)));   // amarillo fuerte
    linea->setPen(QPen(Qt::black, 2));               // borde negro
    linea->setPos(0, 475);                           // centrada en el mismo lugar (480)
    escena->addItem(linea);

    // Personajes (se crean UNA vez con new = memoria dinamica)
    jugador = new Jugador(355, 475, imagenJugador);
    escena->addItem(jugador);

    rival = new Rival(355, 45);
    escena->addItem(rival);

    // Timer: cada 30 ms llama a actualizar()
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);   // ticks parejos = movimiento mas fluido
    connect(timer, &QTimer::timeout, this, &Juego::actualizar);

    nivel = 0;
    dificil = false;
    puntosPorAcierto = 10;
    contador = 0;
    tiempoRestante = 0;
    lanzados = 0;
}

QGraphicsScene* Juego::getEscena()
{
    return escena;
}

void Juego::iniciarNivel(int numeroNivel, bool modoDificil, int segundos)
{
    // EXCEPCION: solo existen los niveles 1 y 2
    if (numeroNivel != 1 && numeroNivel != 2) {
        throw std::invalid_argument("Ese nivel no existe. Solo hay nivel 1 y 2.");
    }

    nivel = numeroNivel;
    dificil = modoDificil;
    contador = 0;
    tiempoRestante = segundos;   // lo escogio en el menu (30, 45 o 60)
    lanzados = 0;

    // DIFICULTAD: en dificil cada acierto vale el doble
    if (dificil) {
        puntosPorAcierto = 20;
    } else {
        puntosPorAcierto = 10;
    }

    // En el nivel 1 se ve la linea; en el nivel 2 se ve el rival
    linea->setVisible(nivel == 1);
    rival->setVisible(nivel == 2);

    actualizarTexto();
    actualizarReloj();
    timer->start(30);
}

void Juego::teclaPresionada(int tecla)
{
    if (nivel == 1) {
        revisarTecla(tecla);
    }
    else if (nivel == 2) {
        // Solo guardamos que la flecha esta presionada;
        // el jugador se mueve suave en cada tick (actualizar)
        if (tecla == Qt::Key_Left) {
            jugador->setIzquierda(true);
        }
        else if (tecla == Qt::Key_Right) {
            jugador->setDerecha(true);
        }
        else if (tecla == Qt::Key_Space) {
            jugador->saltar();
        }
    }
}

void Juego::pausar()
{
    timer->stop();   // sin timer, actualizar() no se llama: 0 trabajo
}

void Juego::reanudar()
{
    // Solo si hay un nivel en curso (nivel 1 o 2) y le queda tiempo
    if (nivel != 0 && tiempoRestante > 0 && !timer->isActive()) {
        timer->start(30);
    }
}

void Juego::teclaSoltada(int tecla)
{
    if (tecla == Qt::Key_Left) {
        jugador->setIzquierda(false);
    }
    else if (tecla == Qt::Key_Right) {
        jugador->setDerecha(false);
    }
}

void Juego::actualizar()
{
    contador = contador + 1;

    // Cada segundo (33 x 30 ms) baja el reloj
    if (contador % 33 == 0) {
        tiempoRestante = tiempoRestante - 1;
        actualizarReloj();
    }

    if (nivel == 1) {
        // Cada segundo sale una nota
        if (contador % 33 == 0) {
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
        // El jugador se mueve, salta y se anima
        jugador->actualizar();

        // Cada 2 segundos el rival lanza un instrumento
        if (contador % 66 == 0) {
            lanzarInstrumento();
        }
        // Cada 6 segundos cae una arepa
        if (contador % 200 == 0) {
            crearArepa();
        }
        // Cada 4 segundos aparece una trampa (maximo 2 a la vez)
        if (contador % 132 == 0 && trampas.size() < 2) {
            crearTrampa();
        }
        for (int i = 0; i < instrumentos.size(); i++) {
            instrumentos[i]->mover();
        }
        for (int i = 0; i < arepas.size(); i++) {
            arepas[i]->mover();
        }
        for (int i = 0; i < trampas.size(); i++) {
            trampas[i]->mover();
        }
        revisarInstrumentos();
        revisarArepas();
        revisarTrampas();

        // Se acaba cuando se acaba el tiempo
        if (tiempoRestante <= 0) {
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
            jugador->sumarPuntos(puntosPorAcierto);
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

void Juego::crearTrampa()
{
    // Lugar al azar en el piso, pero NO encima del jugador (minimo 150 px de distancia)
    double x = std::rand() % 750;
    while (std::abs(x - jugador->getX()) < 150) {
        x = std::rand() % 750;
    }
    Trampa *trampa = new Trampa(x, 575);   // 575 + 25 de alto = piso (600)
    trampas.append(trampa);
    escena->addItem(trampa);
}

void Juego::revisarInstrumentos()
{
    for (int i = instrumentos.size() - 1; i >= 0; i--) {
        Instrumento *instrumento = instrumentos[i];

        // EFICIENCIA: solo revisamos el choque si ya bajo cerca del jugador
        // (revisar choques es costoso; arriba de 400 es imposible que choque)
        bool atrapado = false;
        if (instrumento->y() > 400) {
            atrapado = jugador->collidesWithItem(instrumento);
        }
        bool perdido = instrumento->salioDePantalla(600);

        if (atrapado || perdido) {
            if (atrapado) {
                jugador->sumarPuntos(puntosPorAcierto);
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

        // EFICIENCIA: igual que con los instrumentos
        bool atrapada = false;
        if (arepa->y() > 400) {
            atrapada = jugador->collidesWithItem(arepa);
        }

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

void Juego::revisarTrampas()
{
    for (int i = trampas.size() - 1; i >= 0; i--) {
        Trampa *trampa = trampas[i];

        // EFICIENCIA: primero una resta (barata). Solo si estan cerca
        // revisamos el choque de verdad (collidesWithItem es mas costoso).
        bool cerca = std::abs(trampa->x() - jugador->getX()) < 100;

        // Si la pisa (y no estaba ya congelado): se congela y la trampa desaparece
        bool pisada = cerca && !jugador->estaCongelado() && jugador->collidesWithItem(trampa);
        if (pisada) {
            jugador->congelar(trampa->getCongelamiento());
            emit fallo();
        }

        if (pisada || trampa->seAcabo()) {
            escena->removeItem(trampa);
            delete trampa;
            trampas.removeAt(i);
        }
    }
}

// ===================== PARTE 4: texto y final =====================

void Juego::actualizarTexto()
{
    QString mensaje = "Nivel " + QString::number(nivel)
    + "    Puntos: " + QString::number(jugador->getPuntaje())
        + "    Fallos: " + QString::number(jugador->getFallos());

    if (nivel == 2) {
        mensaje = mensaje + "    Lanzados: " + QString::number(lanzados);
        if (jugador->estaCongelado()) {
            mensaje = mensaje + "    ¡CONGELADO!";
        }
    }
    // EFICIENCIA: solo redibujamos el texto si cambio.
    // (actualizar() se llama 33 veces por segundo, pero el texto casi nunca cambia)
    if (mensaje != texto->toPlainText()) {
        texto->setPlainText(mensaje);
    }
}

void Juego::actualizarReloj()
{
    // Formato minutos:segundos, por ejemplo 0:45
    int minutos = tiempoRestante / 60;
    int segundos = tiempoRestante % 60;
    QString textoReloj = QString::number(minutos) + ":"
                         + QString::number(segundos).rightJustified(2, '0');
    reloj->setPlainText(textoReloj);

    // Los ultimos 5 segundos se pone rojo
    if (tiempoRestante <= 5) {
        reloj->setDefaultTextColor(Qt::red);
    } else {
        reloj->setDefaultTextColor(Qt::white);
    }
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

    for (int i = 0; i < trampas.size(); i++) {
        escena->removeItem(trampas[i]);
        delete trampas[i];
    }
    trampas.clear();
}