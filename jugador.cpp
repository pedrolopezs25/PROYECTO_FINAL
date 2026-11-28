#include "jugador.h"
#include <QTransform>
#include <cmath>

// Duracion de la celebracion: 10 ticks = 0.3 segundos
const int DURACION_CELEBRACION = 10;
const double PI = 3.14159265;

Jugador::Jugador(double x, double y, const QString &rutaImagen)
    : Personaje(x, y, rutaImagen, QString(rutaImagen).replace(".png", "_lado.png"))
// la imagen de lado tiene el mismo nombre con "_lado": personaje1_lado.png
{
    velocidad = 7;      // 7 pixeles por tick = unos 230 pixeles por segundo
    carga = 0;
    puntaje = 0;
    fallos = 0;

    izquierda = false;
    derecha = false;
    direccion = 1;
    paso = 0;

    altura = 0;
    velSalto = 0;
    gravedad = 0.8;

    ticksCongelado = 0;
    ticksCelebrando = 0;
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

void Jugador::celebrar()
{
    ticksCelebrando = DURACION_CELEBRACION;
}

void Jugador::congelar(int ticks)
{
    ticksCongelado = ticks;
    setOpacity(0.5);   // se ve medio transparente mientras esta congelado
}

bool Jugador::estaCongelado() const
{
    return ticksCongelado > 0;
}

bool Jugador::estaEnElPiso() const
{
    return altura == 0;
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
        // No se sale de la pantalla (800 de ancho - lo que mide el jugador)
        if (posX < 0) {
            posX = 0;
        }
        if (posX > 800 - ANCHO) {
            posX = 800 - ANCHO;
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
        mirar(direccion);   // mira hacia donde va (funcion del padre)

        // Rebote de los pasos (solo si esta en el piso)
        if (caminando && altura == 0) {
            paso = paso + 1;
            rebote = std::abs(std::sin(paso * 0.5)) * 6;
        }
    }
    else {
        // QUIETO: vuelve de frente.
        // EFICIENCIA: si ya estaba de frente, cambiarPose no hace nada.
        paso = 0;
        mirar(0);
    }

    // CELEBRACION: se ensancha y da un brinquito, y vuelve a la normalidad.
    // "fase" va de 0 a 1 y sin(pi * fase) sube y baja como una montanita.
    double brinco = 0;
    if (ticksCelebrando > 0) {
        ticksCelebrando = ticksCelebrando - 1;
        double fase = 1.0 - (double)ticksCelebrando / DURACION_CELEBRACION;
        double montanita = std::sin(PI * fase);
        brinco = montanita * 12;                      // sube hasta 12 pixeles

        // Se ensancha un 20% y se aplasta un poquito, estirando desde los pies
        QTransform estirar;
        estirar.translate(ANCHO / 2, ALTO);
        estirar.scale(1 + 0.2 * montanita, 1 - 0.1 * montanita);
        estirar.translate(-ANCHO / 2, -ALTO);
        setTransform(estirar);

        if (ticksCelebrando == 0) {
            setTransform(QTransform());   // vuelve a su tamano normal
        }
    }

    setPos(posX, posY - altura - rebote - brinco);
}

// ===================== Caracteristicas y puntos =====================

void Jugador::comerArepa()
{
    // Caracteristica 1: la arepa le da energia, se mueve mas rapido (sin limite)
    velocidad = velocidad + 4;
}

void Jugador::cargarInstrumento(double peso)
{
    // Caracteristica 2: entre mas carga (y mas pesado), mas lento
    carga = carga + 1;
    velocidad = velocidad - peso;
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

int Jugador::getPuntaje() const
{
    return puntaje;
}

int Jugador::getFallos() const
{
    return fallos;
}