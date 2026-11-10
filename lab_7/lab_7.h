#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab_7.h"
#include "ArchivoContactos.h"

class Lab7 : public QMainWindow
{
    Q_OBJECT

public:
    Lab7(QWidget* parent = nullptr);
    ~Lab7();

private slots:
    void guardarContacto();


    void mostrarRegistrar();
    void mostrarHistorial();
    void mostrarEditar();
    void mostrarEliminar();


    void editarSeleccionado();
    void eliminarSeleccionado();


private:
    Ui::lab_7Class ui;
    ArchivoContactos archivoContactos;
    void cargarTablaContactos();
};