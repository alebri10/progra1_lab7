#include "lab_7.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>

Lab7::Lab7(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Lab 7 — Contactos");

  
    connect(ui.btnGuardar, &QPushButton::clicked, this, &Lab7::guardarContacto);


    connect(ui.actionRegistrar, &QAction::triggered, this, &Lab7::mostrarRegistrar);
    connect(ui.actionHistorial, &QAction::triggered, this, &Lab7::mostrarHistorial);
    connect(ui.actionEditar, &QAction::triggered, this, &Lab7::mostrarEditar);
    connect(ui.actionEliminar, &QAction::triggered, this, &Lab7::mostrarEliminar);

   
    connect(ui.btnEditarSeleccionado, &QPushButton::clicked, this, &Lab7::editarSeleccionado);
    connect(ui.btnEliminarSeleccionado, &QPushButton::clicked, this, &Lab7::eliminarSeleccionado);

    if (ui.tableWidget) {
        ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); 
    }
}

Lab7::~Lab7() {}


void Lab7::mostrarRegistrar() {
    ui.stackedWidget->setCurrentIndex(0); 
}

void Lab7::mostrarHistorial() {
    ui.stackedWidget->setCurrentIndex(1); 
    cargarTablaContactos();
}

void Lab7::mostrarEditar() {
    ui.stackedWidget->setCurrentIndex(1); 
    cargarTablaContactos();
}

void Lab7::mostrarEliminar() {
    ui.stackedWidget->setCurrentIndex(1);
    cargarTablaContactos();
}


void Lab7::cargarTablaContactos() {
    QStringList lineas = archivoContactos.leerContactos();
    ui.tableWidget->setRowCount(0); 
    ui.tableWidget->setColumnCount(3);
    ui.tableWidget->setHorizontalHeaderLabels({ "Nombre", "Teléfono", "Correo" });

    for (int i = 0; i < lineas.size(); ++i) {
        QStringList partes = lineas[i].split(";");
        if (partes.size() == 3) {
            int row = ui.tableWidget->rowCount();
            ui.tableWidget->insertRow(row);
            ui.tableWidget->setItem(row, 0, new QTableWidgetItem(partes[0]));
            ui.tableWidget->setItem(row, 1, new QTableWidgetItem(partes[1]));
            ui.tableWidget->setItem(row, 2, new QTableWidgetItem(partes[2]));
        }
    }
}

void Lab7::guardarContacto() {
    QString nombre = ui.txtNombre->text().trimmed();
    QString telefono = ui.txtTelefono->text().trimmed();
    QString correo = ui.txtCorreo->text().trimmed();
    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Completá los tres campos.");
        return;
    }
    for (const QString& dato : QStringList{ nombre, telefono, correo }) {
        if (dato.contains(';') || dato.contains('\n') || dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido", "No usés punto y coma ni saltos de línea.");
            return;
        }
    }
    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar", "Revisá permisos y espacio disponible.\nArchivo: " + archivoContactos.ruta());
        return;
    }
    ui.txtNombre->clear();
    ui.txtTelefono->clear();
    ui.txtCorreo->clear();
    QMessageBox::information(this, "Guardado completo", "El contacto se guardó correctamente.\nArchivo: " + archivoContactos.ruta());
    ui.txtNombre->setFocus();
}


void Lab7::eliminarSeleccionado() {
    int filaSeleccionada = ui.tableWidget->currentRow();
    if (filaSeleccionada < 0) {
        QMessageBox::warning(this, "Sin selección", "Por favor, seleccioná una fila de la tabla para eliminar.");
        return;
    }

    QMessageBox::StandardButton respuesta = QMessageBox::question(this, "Confirmar eliminación",
        "¿Estás seguro de que querés eliminar este contacto?",
        QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
        QStringList lineas = archivoContactos.leerContactos();
        if (filaSeleccionada < lineas.size()) {
            lineas.removeAt(filaSeleccionada); 
            archivoContactos.reescribirArchivo(lineas); 
            cargarTablaContactos(); 
            QMessageBox::information(this, "Eliminado", "El contacto fue eliminado correctamente.");
        }
    }
}


void Lab7::editarSeleccionado() {
    int filaSeleccionada = ui.tableWidget->currentRow();
    if (filaSeleccionada < 0) {
        QMessageBox::warning(this, "Sin selección", "Por favor, seleccioná una fila de la tabla para editar.");
        return;
    }

    QString nombreActual = ui.tableWidget->item(filaSeleccionada, 0)->text();
    QString telActual = ui.tableWidget->item(filaSeleccionada, 1)->text();
    QString correoActual = ui.tableWidget->item(filaSeleccionada, 2)->text();

   
    ui.txtNombre->setText(nombreActual);
    ui.txtTelefono->setText(telActual);
    ui.txtCorreo->setText(correoActual);
    ui.stackedWidget->setCurrentIndex(0); 
    QStringList lineas = archivoContactos.leerContactos();
    if (filaSeleccionada < lineas.size()) {
    }
}