#include "menuinicio.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>

MenuInicio::MenuInicio(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Festival Vallenato - Menu");

    // Titulo grande
    QLabel *titulo = new QLabel("FESTIVAL VALLENATO", this);
    titulo->setFont(QFont("Arial", 20, QFont::Bold));
    titulo->setAlignment(Qt::AlignCenter);

    // Cajita para escribir el nombre (para la tabla de puntajes)
    QLabel *textoNombre = new QLabel("Tu nombre:", this);
    campoNombre = new QLineEdit(this);
    campoNombre->setPlaceholderText("Escribe tu nombre");
    campoNombre->setMaxLength(15);   // que no sea muy largo

    // Lista para escoger personaje
    QLabel *textoPersonaje = new QLabel("Escoge tu personaje:", this);
    comboPersonaje = new QComboBox(this);
    comboPersonaje->addItem("Goku");             // personaje1.png
    comboPersonaje->addItem("Vegeta");           // personaje2.png
    comboPersonaje->addItem("Piccolo");          // personaje3.png
    comboPersonaje->addItem("Maestro Roshi");    // personaje4.png

    // Lista para escoger dificultad
    QLabel *textoDificultad = new QLabel("Dificultad:", this);
    comboDificultad = new QComboBox(this);
    comboDificultad->addItem("Facil");
    comboDificultad->addItem("Dificil (las notas se mecen mas, puntos x2)");

    // Lista para escoger cuanto dura cada ronda
    QLabel *textoTiempo = new QLabel("Duracion de cada ronda:", this);
    comboTiempo = new QComboBox(this);
    comboTiempo->addItem("30 segundos");
    comboTiempo->addItem("45 segundos");
    comboTiempo->addItem("60 segundos");
    comboTiempo->setCurrentIndex(1);   // por defecto 45

    // Boton para empezar
    QPushButton *botonJugar = new QPushButton("JUGAR", this);
    botonJugar->setFont(QFont("Arial", 14, QFont::Bold));

    // Cuando se presiona JUGAR, el menu se cierra y responde "aceptado"
    connect(botonJugar, &QPushButton::clicked, this, &QDialog::accept);

    // Acomodamos todo en una columna (de arriba hacia abajo)
    QVBoxLayout *columna = new QVBoxLayout(this);
    columna->addWidget(titulo);
    columna->addWidget(textoNombre);
    columna->addWidget(campoNombre);
    columna->addWidget(textoPersonaje);
    columna->addWidget(comboPersonaje);
    columna->addWidget(textoDificultad);
    columna->addWidget(comboDificultad);
    columna->addWidget(textoTiempo);
    columna->addWidget(comboTiempo);
    columna->addWidget(botonJugar);

    setFixedSize(380, 400);
}

QString MenuInicio::getNombre()
{
    // trimmed() quita los espacios de los lados
    QString nombre = campoNombre->text().trimmed();

    // Si no escribio nada, le ponemos un nombre por defecto
    if (nombre.isEmpty()) {
        nombre = "Jugador";
    }
    return nombre;
}

QString MenuInicio::getImagenPersonaje()
{
    // currentIndex() da 0, 1, 2 o 3 segun lo que escogio.
    // Le sumamos 1 para armar el nombre: personaje1.png, personaje2.png...
    int numero = comboPersonaje->currentIndex() + 1;
    return ":/imagenes/personaje" + QString::number(numero) + ".png";
}

int MenuInicio::getDuracion()
{
    // indice 0 = 30 s, 1 = 45 s, 2 = 60 s  ->  30 + indice * 15
    return 30 + comboTiempo->currentIndex() * 15;
}

bool MenuInicio::esDificil()
{
    return comboDificultad->currentIndex() == 1;
}