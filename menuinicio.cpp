#include "menuinicio.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <stdexcept>

MenuInicio::MenuInicio(const TablaPuntajes *tabla, QWidget *parent) : QDialog(parent)
{
    this->tabla = tabla;
    setWindowTitle("Festival Vallenato - Menu");

    // Titulo grande (el "objectName" sirve para darle un estilo propio)
    QLabel *titulo = new QLabel("FESTIVAL VALLENATO", this);
    titulo->setObjectName("titulo");
    titulo->setAlignment(Qt::AlignCenter);

    QLabel *subtitulo = new QLabel("¡Escoge tu personaje y a tocar!", this);
    subtitulo->setObjectName("subtitulo");
    subtitulo->setAlignment(Qt::AlignCenter);

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
    comboPersonaje->addItem("Bulma");            // personaje5.png

    // Foto del personaje: las 5 se cargan aqui UNA vez y luego solo se cambian
    for (int i = 0; i < 5; i++) {
        QString ruta = ":/imagenes/personaje" + QString::number(i + 1) + ".png";
        fotos[i] = QPixmap(ruta).scaled(110, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    foto = new QLabel(this);
    foto->setObjectName("foto");
    foto->setAlignment(Qt::AlignCenter);
    foto->setFixedSize(150, 160);
    mostrarFoto(0);

    // Cuando cambian el personaje, cambia la foto
    connect(comboPersonaje, &QComboBox::currentIndexChanged, this, &MenuInicio::mostrarFoto);

    // Lista para escoger dificultad
    QLabel *textoDificultad = new QLabel("Dificultad:", this);
    comboDificultad = new QComboBox(this);
    comboDificultad->addItem("Facil");
    comboDificultad->addItem("Dificil (puntos x2, mas trampas)");

    // Lista para escoger cuanto dura cada ronda
    QLabel *textoTiempo = new QLabel("Duracion de cada ronda:", this);
    comboTiempo = new QComboBox(this);
    comboTiempo->addItem("30 segundos");
    comboTiempo->addItem("45 segundos");
    comboTiempo->addItem("60 segundos");
    comboTiempo->setCurrentIndex(1);   // por defecto 45

    // Boton para empezar: primero revisa el nombre
    QPushButton *botonJugar = new QPushButton("¡JUGAR!", this);
    botonJugar->setObjectName("jugar");   // estilo solo para este boton
    botonJugar->setCursor(Qt::PointingHandCursor);
    connect(botonJugar, &QPushButton::clicked, this, &MenuInicio::intentarJugar);

    // Columna de la izquierda: los datos.  Derecha: la foto.
    QVBoxLayout *datos = new QVBoxLayout();
    datos->addWidget(textoNombre);
    datos->addWidget(campoNombre);
    datos->addWidget(textoPersonaje);
    datos->addWidget(comboPersonaje);
    datos->addWidget(textoDificultad);
    datos->addWidget(comboDificultad);
    datos->addWidget(textoTiempo);
    datos->addWidget(comboTiempo);

    QHBoxLayout *centro = new QHBoxLayout();
    centro->addLayout(datos);
    centro->addWidget(foto);

    QVBoxLayout *columna = new QVBoxLayout(this);
    columna->addWidget(titulo);
    columna->addWidget(subtitulo);
    columna->addLayout(centro);
    columna->addWidget(botonJugar);

    ponerColores();
    setFixedSize(500, 470);
}

void MenuInicio::ponerColores()
{
    // Hoja de estilo (como CSS de paginas web).
    // Colores sacados del fondo: cielo azul, nubes blancas y dorado de los banderines.
    setStyleSheet(
        "MenuInicio { background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "             stop:0 #2f7fc1, stop:1 #bfe6fb); }"
        "MenuInicio > QLabel { color: #0d2b45; font: bold 11pt 'Arial'; }"
        "QLabel#titulo { background: #0d2b45; color: #ffd23f; font: bold 22pt 'Arial';"
        "                border: 3px solid #ffd23f; border-radius: 12px; padding: 8px; }"
        "QLabel#subtitulo { color: white; font: italic bold 12pt 'Arial'; }"
        "QLabel#foto { background: rgba(255, 255, 255, 170); border: 3px solid #ffd23f;"
        "              border-radius: 14px; }"
        "QLineEdit, QComboBox { background: white; color: #0d2b45; font: 11pt 'Arial';"
        "                       border: 2px solid #2f7fc1; border-radius: 8px; padding: 4px; }"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #f2b632; }"
        "QPushButton#jugar { background: #f2b632; color: #5a2d0c; font: bold 18pt 'Arial';"
        "                    border: 3px solid #c98a12; border-radius: 14px; padding: 8px; }"
        "QPushButton#jugar:hover { background: #ffd23f; }"
        "QPushButton#jugar:pressed { background: #e0a020; }"
        );
}

void MenuInicio::mostrarFoto(int personaje)
{
    foto->setPixmap(fotos[personaje]);
}

void MenuInicio::intentarJugar()
{
    // La TABLA revisa el nombre y lanza una excepcion si esta vacio.
    // Aqui la ATRAPAMOS y le mostramos el mensaje a la persona (el menu sigue abierto).
    try {
        tabla->validarNombre(getNombre());
    }
    catch (std::invalid_argument &error) {
        QMessageBox::warning(this, "Revisa tu nombre", error.what());
        campoNombre->setFocus();
        return;
    }

    // Si el nombre ya esta en la tabla, preguntamos si de verdad es la misma persona
    if (tabla->yaJugo(getNombre())) {
        QMessageBox pregunta(this);
        pregunta.setWindowTitle("Nombre repetido");
        pregunta.setIcon(QMessageBox::Question);
        pregunta.setText("El nombre <b>" + getNombre().toHtmlEscaped()
                         + "</b> ya esta en la tabla de clasificacion.<br><br>"
                           "¿Seguro que quieres jugar con ese nombre?<br>"
                           "<i>Se guarda tu MEJOR puntaje en cada dificultad.</i>");
        QPushButton *botonSi = pregunta.addButton("Si, jugar", QMessageBox::YesRole);
        pregunta.addButton("Cambiar nombre", QMessageBox::NoRole);
        pregunta.exec();

        if (pregunta.clickedButton() != botonSi) {
            campoNombre->setFocus();
            campoNombre->selectAll();   // deja el nombre seleccionado para escribir otro
            return;
        }
    }

    accept();   // todo bien: se cierra el menu y empieza el juego
}

QString MenuInicio::getNombre()
{
    // trimmed() quita los espacios de los lados
    return campoNombre->text().trimmed();
}

QString MenuInicio::getImagenPersonaje()
{
    // currentIndex() da 0, 1, 2, 3 o 4 segun lo que escogio.
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