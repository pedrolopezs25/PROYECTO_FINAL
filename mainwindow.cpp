#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <stdexcept>
#include <QUrl>
#include "menuinicio.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Festival Vallenato");

    // La vista se crea UNA vez; en cada partida solo le cambiamos la escena
    vista = new QGraphicsView(this);
    vista->setFocusPolicy(Qt::NoFocus);   // las teclas las recibe la ventana
    setCentralWidget(vista);
    setFixedSize(820, 620);

    // ----- SONIDOS -----
    // Musica de fondo: se repite sin parar
    musica = new QMediaPlayer(this);
    salidaMusica = new QAudioOutput(this);
    musica->setAudioOutput(salidaMusica);
    musica->setSource(QUrl("qrc:/sonidos/musica.mp3"));
    musica->setLoops(QMediaPlayer::Infinite);
    salidaMusica->setVolume(0.4);
    musica->play();

    // Sonido de acierto
    sonidoAcierto = new QMediaPlayer(this);
    salidaAcierto = new QAudioOutput(this);
    sonidoAcierto->setAudioOutput(salidaAcierto);
    sonidoAcierto->setSource(QUrl("qrc:/sonidos/acierto.mp3"));

    // Sonido de fallo
    sonidoFallo = new QMediaPlayer(this);
    salidaFallo = new QAudioOutput(this);
    sonidoFallo->setAudioOutput(salidaFallo);
    sonidoFallo->setSource(QUrl("qrc:/sonidos/fallo.mp3"));

    juego = nullptr;   // todavia no hay juego
    nuevaPartida();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nuevaPartida()
{
    // MENU DE INICIO: escoger nombre, personaje y dificultad.
    // exec() muestra el menu y espera hasta que presionen JUGAR.
    MenuInicio menu(this);
    menu.exec();
    dificil = menu.esDificil();
    nombreJugador = menu.getNombre();

    // Si ya habia un juego de antes, lo borramos (liberar memoria).
    // deleteLater() lo borra apenas termine lo que esta haciendo, sin peligro.
    if (juego != nullptr) {
        juego->deleteLater();
    }

    // Juego nuevo con el personaje escogido (empieza en 0 puntos)
    juego = new Juego(menu.getImagenPersonaje(), this);
    vista->setScene(juego->getEscena());

    // Cuando el juego avise algo, la ventana responde
    connect(juego, &Juego::nivelTerminado, this, &MainWindow::cuandoTerminaNivel);
    connect(juego, &Juego::acierto, this, &MainWindow::sonarAcierto);
    connect(juego, &Juego::fallo, this, &MainWindow::sonarFallo);

    juego->iniciarNivel(1, dificil);
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
        mostrarTabla(puntaje);
    }
}

void MainWindow::mostrarTabla(int puntaje)
{
    // Metemos el puntaje en la tabla y vemos en que puesto quedo
    int puesto = tabla.agregar(nombreJugador, puntaje);

    QString mensaje = "<b>" + nombreJugador.toHtmlEscaped() + "</b>, hiciste "
                      + QString::number(puntaje) + " puntos.<br>";
    if (puesto > 0) {
        mensaje = mensaje + "Quedaste de puesto " + QString::number(puesto) + ".<br><br>";
    } else {
        mensaje = mensaje + "No alcanzaste a entrar al TOP 10.<br><br>";
    }
    mensaje = mensaje + tabla.comoTexto(puesto);

    // Ventanita con dos botones: jugar otra vez o salir
    QMessageBox ventana(this);
    ventana.setWindowTitle("Tabla de puntajes");
    ventana.setTextFormat(Qt::RichText);   // para que entienda la tabla
    ventana.setText(mensaje);
    QPushButton *botonOtraVez = ventana.addButton("Jugar otra vez", QMessageBox::AcceptRole);
    ventana.addButton("Salir", QMessageBox::RejectRole);
    ventana.exec();

    if (ventana.clickedButton() == botonOtraVez) {
        nuevaPartida();
    } else {
        close();
    }
}

void MainWindow::sonarAcierto()
{
    sonidoAcierto->setPosition(0);   // vuelve al inicio por si ya estaba sonando
    sonidoAcierto->play();
}

void MainWindow::sonarFallo()
{
    sonidoFallo->setPosition(0);
    sonidoFallo->play();
}