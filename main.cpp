#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::srand(std::time(nullptr));   // para que el azar cambie cada vez

    // Si algo falla al crear el juego, lo atrapamos y mostramos el error
    try {
        MainWindow w;
        w.show();
        return a.exec();
    }
    catch (std::exception &error) {
        QMessageBox::critical(nullptr, "Error", error.what());
        return 1;
    }
}