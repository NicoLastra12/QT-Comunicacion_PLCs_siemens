#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "external/snap7/snap7.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Metodo de slot que se activara al presionar conectar a PLC1
    void AlPresionarConectarPLC1();

    // Metodo de slot que se activara al presionar conectar a PLC1
    void AlPresionarDesconectarPLC1();

    // Metodo de slot que se activara al presionar desconectar a PLC2
    void AlPresionarConectarPLC2();

    // Metodo de slot que se activara al presionar desconectar a PLC2
    void AlPresionarDesconectarPLC2();

    // Metodo de slot que llamara el QTimer cada cierto tiempo
    void LeerPLCsCiclicamente();


private:
    //Puntero a la ventana principal
    Ui::MainWindow *ui;

    //Punteros a las conexiones de cada PLC
    TS7Client *plc1;
    TS7Client *plc2;

    //Puntero a cronometro ciclico
    QTimer *timerLectura;

    // Función interna para apagar sockets de forma segura al cerrar
    void cerrarConexionesSeguras();

};
#endif // MAINWINDOW_H
