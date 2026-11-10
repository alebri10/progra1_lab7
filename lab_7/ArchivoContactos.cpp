#include "ArchivoContactos.h" 
#include <QFile> 
#include <QDir> 
#include <QByteArray> 
#include <QTextStream>

QString ArchivoContactos::ruta() const
{
	return QDir::homePath() + "/Lab7Datos/contactos.txt";
}
bool ArchivoContactos::guardar(const QString& nombre, 
	const QString& telefono,
	const QString& correo) const
{
	QString carpeta = QDir::homePath() + "/Lab7Datos"; 
	if (!QDir().mkpath(carpeta))
		return false;

	QFile archivo(ruta()); if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append)) return false;

	QString linea = nombre + ";" + telefono + ";" + correo + "\n"; 
	QByteArray datos = linea.toUtf8(); 
	bool completo = archivo.write(datos) == datos.size(); 
	bool vaciado = archivo.flush(); 
	archivo.close();
	return completo && vaciado;
}

QStringList ArchivoContactos::leerContactos() const {
    QStringList listaLineas;
    QFile archivo(ruta());
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return listaLineas; 

    QTextStream in(&archivo);
    in.setEncoding(QStringConverter::Utf8);
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (!linea.isEmpty()) {
            listaLineas.append(linea);
        }
    }
    archivo.close();
    return listaLineas;
}

bool ArchivoContactos::reescribirArchivo(const QStringList& lineas) const {
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&archivo);
    out.setEncoding(QStringConverter::Utf8);
    for (const QString& linea : lineas) {
        out << linea << "\n";
    }

    bool vaciado = archivo.flush();
    archivo.close();
    return vaciado;
}