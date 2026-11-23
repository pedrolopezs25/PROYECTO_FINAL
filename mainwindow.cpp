#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Festival Vallenato");

    // Creamos el juego y una vista para verlo
    juego = new Juego(this);
    vista = new QGraphicsView(juego->getEscena(), this);
    vista->setFocusPolicy(Qt::NoFocus);   // las teclas las recibe la ventana
    setCentralWidget(vista);
    setFixedSize(820, 620);

    // Cuando el juego avise que termino un nivel, llamamos a cuandoTerminaNivel
    connect(juego, &Juego::nivelTerminado, this, &MainWindow::cuandoTerminaNivel);

    // Preguntamos la dificultad (cambia que tanto se mecen las notas)
    QMessageBox::StandardButton respuesta = QMessageBox::question(
        this, "Dificultad", "¿Quieres jugar en modo DIFICIL?\n(las notas se mecen mas)");
    dificil = (respuesta == QMessageBox::Yes);

    juego->iniciarNivel(1, dificil);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    // La ventana no decide nada: solo le pasa la tecla al juego
    juego->teclaPresionada(evento->key());
}

void MainWindow::cuandoTerminaNivel(int numeroNivel, int puntaje)
{
    if (numeroNivel == 1) {
        QMessageBox::information(this, "Nivel 1 terminado",
                                 "Puntos: " + QString::number(puntaje) +
                                     "\n\nNivel 2: atrapa los instrumentos con las flechas.");
        juego->iniciarNivel(2, dificil);
    } else {
        QMessageBox::information(this, "Fin del festival",
                                 "Puntaje final: " + QString::number(puntaje));
        close();
    }
}