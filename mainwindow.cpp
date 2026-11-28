#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <stdexcept>
#include <QUrl>
#include "menuinicio.h"
#include <QPainter>
#include <QFile>
#include <QTimer>
#include <QStringList>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Festival Vallenato");
    ponerEstilo();   // primero el estilo, para que hasta los errores salgan decorados

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

    // Antes de jugar revisamos que esten todos los archivos.
    // Si falta alguno se lanza una excepcion y main() la atrapa y muestra el error.
    verificarRecursos();

    // TABLA DE PUNTAJES: se lee del archivo puntajes.txt (al lado del .exe).
    // Si no se puede leer, avisamos pero se puede jugar igual.
    try {
        tabla.cargar(QCoreApplication::applicationDirPath() + "/puntajes.txt");
    }
    catch (std::runtime_error &error) {
        QMessageBox::warning(this, "Tabla de puntajes", error.what());
    }

    juego = nullptr;   // todavia no hay juego
    nuevaPartida();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ponerEstilo()
{
    // Hoja de estilo para TODA la aplicacion (qApp = el programa entero).
    // Asi todos los QMessageBox (avisos, errores, preguntas, la tabla)
    // salen con los colores del fondo: cielo azul, blanco y dorado.
    qApp->setStyleSheet(
        "QMessageBox { background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "              stop:0 #bfe6fb, stop:1 #ffffff);"
        "              border: 3px solid #ffd23f; }"
        "QMessageBox QLabel { color: #0d2b45; font: 11pt 'Arial'; }"
        "QMessageBox QPushButton { background: #f2b632; color: #5a2d0c;"
        "              font: bold 11pt 'Arial'; border: 2px solid #c98a12;"
        "              border-radius: 10px; padding: 6px 16px; min-width: 90px; }"
        "QMessageBox QPushButton:hover { background: #ffd23f; }"
        "QMessageBox QPushButton:pressed { background: #e0a020; }"
        );
}

void MainWindow::verificarRecursos()
{
    // 1) Imagenes de los objetos: se cargan UNA vez a la cache (eficiencia).
    //    Si falta alguna, lanza std::runtime_error.
    Juego::precargarImagenes();

    // 2) Fondo, personajes y sonidos: revisamos que existan
    QStringList archivos;
    archivos << ":/imagenes/fondo.png" << ":/imagenes/rival.png" << ":/imagenes/rival_lado.png"
             << ":/sonidos/musica.mp3" << ":/sonidos/acierto.mp3" << ":/sonidos/fallo.mp3";
    for (int i = 1; i <= 5; i++) {   // los 5 personajes del menu
        archivos << ":/imagenes/personaje" + QString::number(i) + ".png";
        archivos << ":/imagenes/personaje" + QString::number(i) + "_lado.png";
    }

    for (const QString &archivo : archivos) {   // const & = no copia cada texto
        if (!QFile::exists(archivo)) {
            throw std::runtime_error(("No se encontro el archivo: " + archivo).toStdString());
        }
    }
}

void MainWindow::nuevaPartida()
{
    // MENU DE INICIO: escoger nombre, personaje y dificultad.
    // Le pasamos la tabla para que revise si el nombre ya jugo.
    // exec() muestra el menu y espera hasta que presionen JUGAR.
    MenuInicio menu(&tabla, this);
    if (menu.exec() != QDialog::Accepted) {
        // Cerraron el menu con la X: cerramos el juego
        // (singleShot espera a que la ventana este lista para cerrarla)
        QTimer::singleShot(0, this, &MainWindow::close);
        return;
    }
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
    empezarNivel(1);
}

void MainWindow::empezarNivel(int numeroNivel)
{
    // iniciarNivel lanza una excepcion si el nivel o la duracion no son validos.
    // La atrapamos AQUI para mostrar el error sin que el programa se caiga.
    try {
        juego->iniciarNivel(numeroNivel, dificil, duracion);
    }
    catch (std::invalid_argument &error) {
        QMessageBox::critical(this, "Error", error.what());
        QTimer::singleShot(0, this, &MainWindow::close);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    // Si se deja la tecla presionada, Windows la "repite" sola.
    // Esas repeticiones las ignoramos: el movimiento ya es continuo.
    if (evento->isAutoRepeat() || juego == nullptr) {
        return;
    }
    // La ventana no decide nada: solo le pasa la tecla al juego
    juego->teclaPresionada(evento->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat() || juego == nullptr) {
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
        empezarNivel(2);
    } else {
        mostrarTabla(puntaje);
    }
}

void MainWindow::mostrarTabla(int puntaje)
{
    // Antes de guardar, miramos cual era su record en esta dificultad
    int recordAnterior = tabla.mejorPuntaje(nombreJugador, dificil);
    int puesto = tabla.agregar(nombreJugador, dificil, duracion, puntaje);

    // Guardamos en el archivo (solo escribe si hubo un record nuevo)
    try {
        tabla.guardar();
    }
    catch (std::runtime_error &error) {
        QMessageBox::warning(this, "Tabla de puntajes", error.what());
    }

    QString textoDificultad = "Facil";
    if (dificil) {
        textoDificultad = "Dificil";
    }

    QString mensaje = "<h2 style='color:#0d2b45'>TABLA DE CAMPEONES</h2>"
                      "<b>" + nombreJugador.toHtmlEscaped() + "</b>, hiciste <b>"
                      + QString::number(puntaje) + "</b> puntos en " + textoDificultad + ".<br>";

    if (recordAnterior >= 0 && puntaje <= recordAnterior) {
        // Ya habia jugado y no supero su record: se queda el anterior
        mensaje += "Tu record en " + textoDificultad + " sigue siendo <b>"
                   + QString::number(recordAnterior) + "</b> (puesto "
                   + QString::number(puesto) + ").<br><br>";
    } else if (recordAnterior >= 0) {
        mensaje += "<b>¡NUEVO RECORD!</b> Antes tenias " + QString::number(recordAnterior)
                   + ". Ahora vas de puesto <b>" + QString::number(puesto) + "</b>.<br><br>";
    } else {
        mensaje += "Quedaste de puesto <b>" + QString::number(puesto) + "</b>.<br><br>";
    }
    mensaje += tabla.comoTexto(nombreJugador, dificil);

    // Ventanita con dos botones: jugar otra vez o salir (el estilo lo pone ponerEstilo)
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