#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <stdexcept>
#include <QUrl>
#include "menuinicio.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Festival Vallenato");

    // La vista se crea UNA vez; en cada partida solo le cambiamos la escena
    vista = new QGraphicsView(this);
    vista->setFocusPolicy(Qt::NoFocus);   // las teclas las recibe la ventana
    vista->setRenderHint(QPainter::SmoothPixmapTransform);   // imagenes suaves al girar
    vista->setRenderHint(QPainter::Antialiasing);
    // EFICIENCIA: el fondo (800x600) se pinta UNA vez y se guarda,
    // en vez de volver a pintarlo 33 veces por segundo
    vista->setCacheMode(QGraphicsView::CacheBackground);
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
    duracion = menu.getDuracion();

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

    reiniciarMusica();   // la cancion arranca desde el principio
    juego->iniciarNivel(1, dificil, duracion);
}

void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    // Si se deja la tecla presionada, Windows la "repite" sola.
    // Esas repeticiones las ignoramos: el movimiento ya es continuo.
    if (evento->isAutoRepeat()) {
        return;
    }
    // La ventana no decide nada: solo le pasa la tecla al juego
    juego->teclaPresionada(evento->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) {
        return;
    }
    juego->teclaSoltada(evento->key());
}

void MainWindow::changeEvent(QEvent *evento)
{
    // EFICIENCIA: si minimizan la ventana, pausamos el juego y la musica
    if (evento->type() == QEvent::WindowStateChange && juego != nullptr) {
        if (isMinimized()) {
            juego->pausar();
            musica->pause();
        } else {
            juego->reanudar();
            musica->play();
        }
    }
    QMainWindow::changeEvent(evento);
}

void MainWindow::cuandoTerminaNivel(int numeroNivel, int puntaje)
{
    if (numeroNivel == 1) {
        QMessageBox::information(this, "Nivel 1 terminado",
                                 "Puntos: " + QString::number(puntaje) +
                                     "\n\nNivel 2: atrapa los instrumentos con las flechas"
                                     "\ny salta las trampas con ESPACIO.");
        reiniciarMusica();   // nuevo nivel = cancion desde el principio
        juego->iniciarNivel(2, dificil, duracion);
    } else {
        mostrarTabla(puntaje);
    }
}

void MainWindow::mostrarTabla(int puntaje)
{
    // Metemos el puntaje en la tabla y vemos en que puesto quedo
    QString textoDificultad = "Facil";
    if (dificil) {
        textoDificultad = "Dificil";
    }
    int puesto = tabla.agregar(nombreJugador, textoDificultad, duracion, puntaje);

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

void MainWindow::reiniciarMusica()
{
    musica->stop();          // la paramos (por si se habia cortado)
    musica->setPosition(0);  // la devolvemos al segundo 0
    musica->play();          // y suena otra vez desde el principio
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