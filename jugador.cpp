#include "jugador.h"
#include <QBrush>
#include <QPen>
#include <QTransform>
#include <cmath>

Jugador::Jugador(double x, double y, QString rutaImagen)
{
    posX = x;
    posY = y;
    velocidad = 7;      // 7 pixeles por tick = unos 230 pixeles por segundo
    carga = 0;
    puntaje = 0;
    fallos = 0;

    izquierda = false;
    derecha = false;
    altura = 0;
    velSalto = 0;
    gravedad = 0.8;
    ticksCongelado = 0;

    setRect(0, 0, 90, 120);             // tamano del personaje
    setBrush(QBrush(Qt::blue));
    setPos(posX, posY);
    setTransformOriginPoint(45, 120);   // si se inclina, gira desde los pies

    // ----- Cargamos las poses UNA sola vez (eficiencia) -----
    // De frente: la imagen que escogio en el menu (personaje1.png...)
    poseFrente = QPixmap(rutaImagen);

    // De lado: el mismo nombre con "_lado" (personaje1_lado.png...)
    QString rutaLado = rutaImagen;
    rutaLado.replace(".png", "_lado.png");
    poseDerecha = QPixmap(rutaLado);
    tieneLado = !poseDerecha.isNull();

    if (tieneLado) {
        poseDerecha = poseDerecha.scaled(90, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // Para la izquierda NO hace falta otra imagen: volteamos la de la derecha.
        // scale(-1, 1) = espejo horizontal
        poseIzquierda = poseDerecha.transformed(QTransform().scale(-1, 1));
    }

    imagen = nullptr;
    poseActual = 0;
    if (!poseFrente.isNull()) {
        poseFrente = poseFrente.scaled(90, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imagen = new QGraphicsPixmapItem(this);   // hija: se mueve con el
        imagen->setTransformationMode(Qt::SmoothTransformation);
        poseActual = -1;                          // para que cambiarPose la ponga
        cambiarPose(0);
        setPen(Qt::NoPen);
        setBrush(Qt::NoBrush);
    }

    direccion = 1;
    paso = 0;
}

// ===================== Controles =====================

void Jugador::setIzquierda(bool presionada)
{
    izquierda = presionada;
}

void Jugador::setDerecha(bool presionada)
{
    derecha = presionada;
}

void Jugador::saltar()
{
    // Solo puede saltar si esta en el piso y no esta congelado
    if (altura == 0 && ticksCongelado == 0) {
        velSalto = 13;   // impulso hacia arriba
    }
}

void Jugador::congelar(int ticks)
{
    ticksCongelado = ticks;
    setOpacity(0.5);   // se ve medio transparente mientras esta congelado
}

bool Jugador::estaCongelado()
{
    return ticksCongelado > 0;
}

// ===================== Cada tick =====================

void Jugador::actualizar()
{
    // 1) CONGELADO: no se mueve hasta que se le acabe el tiempo
    if (ticksCongelado > 0) {
        ticksCongelado = ticksCongelado - 1;
        if (ticksCongelado == 0) {
            setOpacity(1);   // vuelve a verse normal
        }
    }
    else {
        // 2) MOVIMIENTO SUAVE: un poquito en cada tick mientras la flecha siga presionada
        if (izquierda && !derecha) {
            posX = posX - velocidad;
            direccion = -1;
        }
        if (derecha && !izquierda) {
            posX = posX + velocidad;
            direccion = 1;
        }
        // No se sale de la pantalla (800 de ancho - 90 del jugador)
        if (posX < 0) {
            posX = 0;
        }
        if (posX > 710) {
            posX = 710;
        }
    }

    // 3) SALTO (tiro vertical): altura += velocidad,  velocidad -= gravedad
    if (altura > 0 || velSalto > 0) {
        altura = altura + velSalto;
        velSalto = velSalto - gravedad;
        if (altura <= 0) {   // toco el piso otra vez
            altura = 0;
            velSalto = 0;
        }
    }

    // 4) ANIMACION
    animar();
}

void Jugador::animar()
{
    bool caminando = (izquierda != derecha) && ticksCongelado == 0;
    double rebote = 0;

    if (caminando || altura > 0) {
        // Mira hacia donde va
        if (tieneLado) {
            if (direccion == 1) {
                cambiarPose(1);
            } else {
                cambiarPose(2);
            }
        } else {
            setRotation(direccion * 10);   // sin imagen de lado: se inclina
        }

        // Rebote de los pasos (solo si esta en el piso)
        if (caminando && altura == 0) {
            paso = paso + 1;
            rebote = std::abs(std::sin(paso * 0.5)) * 6;
        }
    }
    else if (paso != 0 || poseActual != 0) {
        // SE ACABA DE QUEDAR QUIETO: vuelve de frente y derechito.
        // EFICIENCIA: esto se hace UNA sola vez, no en cada tick.
        paso = 0;
        setRotation(0);
        cambiarPose(0);
    }

    setPos(posX, posY - altura - rebote);
}

void Jugador::cambiarPose(int nuevaPose)
{
    // EFICIENCIA: si ya tiene esa pose, no hacemos nada
    if (imagen == nullptr || nuevaPose == poseActual) {
        return;
    }
    poseActual = nuevaPose;

    if (nuevaPose == 1) {
        imagen->setPixmap(poseDerecha);
    } else if (nuevaPose == 2) {
        imagen->setPixmap(poseIzquierda);
    } else {
        imagen->setPixmap(poseFrente);
    }

    // Centramos la imagen y la ponemos con los pies abajo
    double ancho = imagen->pixmap().width();
    double alto = imagen->pixmap().height();
    imagen->setPos((90 - ancho) / 2, 120 - alto);
}

// ===================== Caracteristicas y puntos =====================

void Jugador::comerArepa()
{
    // Caracteristica 1: la arepa le da energia, se mueve mas rapido
    velocidad = velocidad + 2;
    if (velocidad > 13) {
        velocidad = 13;             // limite para que no sea exagerado
    }
}

void Jugador::cargarInstrumento()
{
    // Caracteristica 2: entre mas carga, mas lento
    carga = carga + 1;
    velocidad = velocidad - 0.5;
    if (velocidad < 3) {
        velocidad = 3;              // nunca se queda quieto del todo
    }
}

void Jugador::sumarPuntos(int puntos)
{
    puntaje = puntaje + puntos;
}

void Jugador::sumarFallo()
{
    fallos = fallos + 1;
}

int Jugador::getPuntaje()
{
    return puntaje;
}

int Jugador::getFallos()
{
    return fallos;
}

double Jugador::getX()
{
    return posX;
}