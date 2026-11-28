QT += widgets
QT += multimedia
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arepa.cpp \
    instrumento.cpp \
    juego.cpp \
    jugador.cpp \
    main.cpp \
    mainwindow.cpp \
    menuinicio.cpp \
    nota.cpp \
    objeto.cpp \
    rival.cpp \
    tablapuntajes.cpp \
    trampa.cpp

HEADERS += \
    arepa.h \
    instrumento.h \
    juego.h \
    jugador.h \
    mainwindow.h \
    menuinicio.h \
    nota.h \
    objeto.h \
    rival.h \
    tablapuntajes.h \
    trampa.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imagenes.qrc \
    sonidos.qrc
