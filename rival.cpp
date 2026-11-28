#include "rival.h"
#include <cstdlib>
#include <cmath>

const double PI = 3.14159265;

Rival::Rival(double x, double y)
    : Personaje(x, y, ":/imagenes/rival.png", ":/imagenes/rival_lado.png")
{
    ultimaX = 0;
    ultimoAtrapado = true;
    for (int i = 0; i < 3; i++) {
        fallos[i] = 0;      // al inicio no sabe nada
    }
    ticksMirando = 0;
}

// 1. PERCIBIR: se fija donde cayo el instrumento y si lo atraparon
void Rival::percibir(double xCaida, bool atrapado)
{
    ultimaX = xCaida;
    ultimoAtrapado = atrapado;
}

// 2. APRENDER: si el jugador fallo, lo anota en la zona donde cayo
void Rival::aprender()
{
    if (ultimoAtrapado == false) {
        // La pantalla mide 800: cada zona mide unos 267 pixeles
        int zona = (int)(ultimaX / 267);
        if (zona < 0) {
            zona = 0;
        }
        if (zona > 2) {
            zona = 2;
        }
        fallos[zona] = fallos[zona] + 1;
    }
}

// 3. RAZONAR: escoge la zona donde el jugador falla mas
int Rival::razonar()
{
    int mejor = std::rand() % 3;   // si nadie gana, escoge al azar
    for (int zona = 0; zona < 3; zona++) {
        if (fallos[zona] > fallos[mejor]) {
            mejor = zona;
        }
    }
    return mejor;
}

// 4. ACTUAR: calcula el angulo, se voltea y lanza
Instrumento* Rival::actuar(bool dificil)
{
    // Angulo en grados: 0 = derecha, 90 = hacia abajo, 180 = izquierda,
    // 270 = hacia arriba (rebota en el techo)
    int angulo;

    if (std::rand() % 4 == 0) {
        // 1 de cada 4: EXPLORA, lanza en cualquier direccion (0 a 359 grados)
        angulo = std::rand() % 360;
    } else {
        int zona = razonar();
        if (zona == 0) {
            angulo = 110 + std::rand() % 50;   // abajo a la izquierda (110 a 159)
        } else if (zona == 1) {
            angulo = 70 + std::rand() % 41;    // casi derecho hacia abajo (70 a 110)
        } else {
            angulo = 20 + std::rand() % 50;    // abajo a la derecha (20 a 69)
        }
    }

    // Rapidez: en dificil los tira mas rapido
    double rapidez = 5 + std::rand() % 3;     // 5, 6 o 7
    if (dificil) {
        rapidez = rapidez * 1.4;
    }

    // Separamos la rapidez en x y en y con coseno y seno
    double radianes = angulo * PI / 180;
    double velX = rapidez * std::cos(radianes);
    double velY = rapidez * std::sin(radianes);

    // Se voltea hacia donde lanza (si es casi vertical, se queda de frente)
    if (velX > 1) {
        mirar(1);
    } else if (velX < -1) {
        mirar(-1);
    } else {
        mirar(0);
    }
    ticksMirando = 20;   // se queda asi 20 ticks (0.6 segundos)

    int tipo = std::rand() % 3;               // guacharaca, caja o acordeon
    bool rebota = (std::rand() % 3 == 0);     // 1 de cada 3 rebota en el piso

    // Sale desde las manos (centro del rival)
    double x = posX + ANCHO / 2 - Instrumento::TAMANO / 2;
    double y = posY + ALTO / 2;
    return new Instrumento(x, y, velX, velY, tipo, rebota);
}

void Rival::actualizar()
{
    // Cuando se acaba el tiempo, vuelve a mirar de frente (una sola vez)
    if (ticksMirando > 0) {
        ticksMirando = ticksMirando - 1;
        if (ticksMirando == 0) {
            mirar(0);
        }
    }
}