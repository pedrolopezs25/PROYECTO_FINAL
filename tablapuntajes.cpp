#include "tablapuntajes.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <stdexcept>

TablaPuntajes::TablaPuntajes()
{
    maximo = 10;         // en pantalla mostramos el TOP 10
    hayCambios = false;
}

// ===================== Archivo =====================

void TablaPuntajes::cargar(const QString &rutaArchivo)
{
    ruta = rutaArchivo;

    QFile archivo(ruta);
    if (!archivo.exists()) {
        return;   // primera vez que se juega: no hay archivo todavia, no es error
    }

    // EXCEPCION: el archivo existe pero no se pudo abrir
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error(("No se pudo leer el archivo de puntajes: " + ruta).toStdString());
    }

    // Cada linea es:  nombre;dificultad;segundos;puntaje   (ej:  Erika;1;45;1000)
    QTextStream entrada(&archivo);
    while (!entrada.atEnd()) {
        QStringList partes = entrada.readLine().split(';');
        if (partes.size() != 4) {
            continue;   // linea danada: la saltamos
        }

        Registro registro;
        registro.nombre = partes[0];
        registro.dificil = (partes[1] == "1");
        registro.segundos = partes[2].toInt();
        registro.puntaje = partes[3].toInt();

        // El archivo ya viene ordenado, pero insertarOrdenado lo asegura
        insertarOrdenado(registro);
        mejores.insert(armarLlave(registro.nombre, registro.dificil), registro.puntaje);
        nombresUsados.insert(registro.nombre.toLower());
    }
    archivo.close();
}

void TablaPuntajes::guardar()
{
    // EFICIENCIA: si nada cambio, no escribimos el archivo
    if (!hayCambios || ruta.isEmpty()) {
        return;
    }

    QFile archivo(ruta);
    // Truncate = borra lo que habia y escribe todo de nuevo (ya ordenado)
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        throw std::runtime_error(("No se pudo guardar el archivo de puntajes: " + ruta).toStdString());
    }

    QTextStream salida(&archivo);
    for (const Registro &registro : registros) {   // const & = no copia cada registro
        salida << registro.nombre << ";" << (registro.dificil ? 1 : 0) << ";"
               << registro.segundos << ";" << registro.puntaje << "\n";
    }
    archivo.close();
    hayCambios = false;
}

// ===================== Nombres =====================

void TablaPuntajes::validarNombre(const QString &nombre) const
{
    // EXCEPCION 1: nombre vacio
    if (nombre.isEmpty()) {
        throw std::invalid_argument("Escribe tu nombre para poder jugar.");
    }
    // EXCEPCION 2: el ";" se usa para separar los datos en el archivo
    if (nombre.contains(';')) {
        throw std::invalid_argument("El nombre no puede tener el simbolo ;");
    }
}

bool TablaPuntajes::yaJugo(const QString &nombre) const
{
    // toLower(): "Erika" y "erika" cuentan como el mismo nombre
    return nombresUsados.contains(nombre.toLower());
}

QString TablaPuntajes::armarLlave(const QString &nombre, bool dificil)
{
    return nombre.toLower() + "|" + QString::number(dificil);
}

int TablaPuntajes::mejorPuntaje(const QString &nombre, bool dificil) const
{
    // value(llave, -1): si no existe, devuelve -1
    return mejores.value(armarLlave(nombre, dificil), -1);
}

// ===================== Puntajes =====================

int TablaPuntajes::agregar(const QString &nombre, bool dificil, int segundos, int puntaje)
{
    nombresUsados.insert(nombre.toLower());
    QString llave = armarLlave(nombre, dificil);

    // Buscamos si ya tenia un renglon en esta dificultad
    int viejo = -1;
    if (mejores.contains(llave)) {   // EFICIENCIA: solo recorremos si sabemos que existe
        for (int i = 0; i < registros.size(); i++) {
            if (registros[i].dificil == dificil
                && registros[i].nombre.toLower() == nombre.toLower()) {
                viejo = i;
                break;
            }
        }
    }

    // No supero su record: la tabla no cambia, devolvemos su puesto actual
    if (viejo >= 0 && puntaje <= registros[viejo].puntaje) {
        return viejo + 1;
    }

    // Supero su record (o es la primera vez): quitamos el viejo y metemos el nuevo
    if (viejo >= 0) {
        registros.removeAt(viejo);
    }
    mejores.insert(llave, puntaje);

    Registro nuevo;
    nuevo.nombre = nombre;
    nuevo.dificil = dificil;
    nuevo.segundos = segundos;
    nuevo.puntaje = puntaje;

    hayCambios = true;   // hay que guardar en el archivo
    return insertarOrdenado(nuevo);
}

int TablaPuntajes::insertarOrdenado(const Registro &nuevo)
{
    // EFICIENCIA: como la lista ya esta ordenada, no hay que ordenarla toda.
    // Solo buscamos el primer puesto con menos puntos y metemos el nuevo ahi.
    int posicion = 0;
    while (posicion < registros.size() && registros[posicion].puntaje >= nuevo.puntaje) {
        posicion = posicion + 1;
    }
    registros.insert(posicion, nuevo);
    return posicion + 1;   // puesto empezando en 1
}

QString TablaPuntajes::comoTexto(const QString &nombreResaltado, bool dificilResaltado) const
{
    // Tabla HTML con los colores del fondo: azul cielo, blanco y dorado
    QString texto = "<table cellspacing='0' cellpadding='5' width='100%'>";
    texto += "<tr bgcolor='#0d2b45'>"
             "<th><font color='#ffd23f'>Puesto</font></th>"
             "<th><font color='white'>Nombre</font></th>"
             "<th><font color='white'>Dificultad</font></th>"
             "<th><font color='white'>Tiempo</font></th>"
             "<th><font color='#ffd23f'>Puntos</font></th></tr>";

    for (int i = 0; i < registros.size() && i < maximo; i++) {
        const Registro &registro = registros[i];   // & = sin copiar

        // El record del que acaba de jugar sale en dorado;
        // los demas, un renglon blanco y uno azulito
        QString color = "#ffffff";
        if (registro.dificil == dificilResaltado
            && registro.nombre.toLower() == nombreResaltado.toLower()) {
            color = "#ffd23f";
        } else if (i % 2 == 1) {
            color = "#dcefff";
        }

        QString dificultad = "Facil";
        if (registro.dificil) {
            dificultad = "Dificil";
        }

        // += agrega al final sin crear una copia nueva del texto (eficiencia)
        texto += "<tr bgcolor='" + color + "'>"
                                           "<td align='center'><b>" + QString::number(i + 1) + "</b></td>"
                                            "<td>" + registro.nombre.toHtmlEscaped() + "</td>"
                                                     "<td align='center'>" + dificultad + "</td>"
                                "<td align='center'>" + QString::number(registro.segundos) + " s</td>"
                                                        "<td align='center'><b>" + QString::number(registro.puntaje) + "</b></td></tr>";
    }

    texto += "</table>";
    return texto;
}