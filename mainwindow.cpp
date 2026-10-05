#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Conexion PLCs siemens");


    // Inicializamos conexiones con cada PLC
    plc1 = new TS7Client();
    plc2 = new TS7Client();

    // Se conectan los botones de la pantalla
    connect(ui->PB_conectar_PLC1, &QPushButton::clicked, this, &MainWindow::AlPresionarConectarPLC1);
    connect(ui->PB_desconectar_PLC1, &QPushButton::clicked, this, &MainWindow::AlPresionarDesconectarPLC1);
    connect(ui->PB_conectar_PLC2, &QPushButton::clicked, this, &MainWindow::AlPresionarConectarPLC2);
    connect(ui->PB_desconectar_PLC2, &QPushButton::clicked, this, &MainWindow::AlPresionarDesconectarPLC2);

    // Estado inicial de la pantalla
    ui->PB_desconectar_PLC1->setEnabled(false);
    ui->PB_desconectar_PLC2->setEnabled(false);
    ui->statusbar->showMessage("Listo para conectar.");

    // Inicializar el timer cíclico
    timerLectura = new QTimer(this);
    connect(timerLectura, &QTimer::timeout, this, &MainWindow::LeerPLCsCiclicamente);
    timerLectura->start(500); // 500 milisegundos
}

MainWindow::~MainWindow()
{
    cerrarConexionesSeguras(); // Apaga los sockets de comunicación
    delete plc1;
    delete plc2;
    delete timerLectura;
    delete ui;
}


// =========================================================================
// LÓGICA DE CONEXIÓN / DESCONEXIÓN (PLC 1)
// =========================================================================

void MainWindow::AlPresionarConectarPLC1() {
    QString ip = ui->TXT_IP_PLC1_2->text().trimmed();
    if (ip.isEmpty()) {
        ui->statusbar->setStyleSheet("color: red;");
        ui->statusbar->showMessage("Error: Ingresa la IP del PLC 1.");
        return;
    }

    ui->statusbar->setStyleSheet("color: orange;");
    ui->statusbar->showMessage("Conectando a PLC 1 (" + ip + ")...");
    ui->PB_conectar_PLC1->setEnabled(false);

    int res = plc1->ConnectTo(ip.toStdString().c_str(), 0, 1); // Rack 0, Slot 1

    if (res == 0) {
        ui->statusbar->setStyleSheet("color: green;");
        ui->statusbar->showMessage("¡PLC 1 Conectado con éxito!");
        ui->PB_desconectar_PLC1->setEnabled(true);
    } else {
        ui->statusbar->setStyleSheet("color: red;");
        ui->statusbar->showMessage("Error PLC 1. Código: " + QString::number(res));
        ui->PB_conectar_PLC1->setEnabled(true);
    }
}

void MainWindow::AlPresionarDesconectarPLC1() {
    if (plc1 && plc1->Connected()) plc1->Disconnect();
    ui->PB_conectar_PLC1->setEnabled(true);
    ui->PB_desconectar_PLC1->setEnabled(false);
    ui->statusbar->setStyleSheet("color: black;");
    ui->statusbar->showMessage("PLC 1 Desconectado.");
}



// =========================================================================
// LÓGICA DE CONEXIÓN / DESCONEXIÓN (PLC 2)
// =========================================================================


void MainWindow::AlPresionarConectarPLC2() {
    QString ip = ui->TXT_IP_PLC2_2->text().trimmed();
    if (ip.isEmpty()) {
        ui->statusbar->setStyleSheet("color: red;");
        ui->statusbar->showMessage("Error: Ingresa la IP del PLC 2.");
        return;
    }

    ui->statusbar->setStyleSheet("color: orange;");
    ui->statusbar->showMessage("Conectando a PLC 2 (" + ip + ")...");
    ui->PB_conectar_PLC2->setEnabled(false);

    int res = plc2->ConnectTo(ip.toStdString().c_str(), 0, 2); // Rack 0, Slot 2

    if (res == 0) {
        ui->statusbar->setStyleSheet("color: green;");
        ui->statusbar->showMessage("¡PLC 2 Conectado con éxito!");
        ui->PB_desconectar_PLC2->setEnabled(true);
    } else {
        ui->statusbar->setStyleSheet("color: red;");
        ui->statusbar->showMessage("Error PLC 2. Código: " + QString::number(res));
        ui->PB_conectar_PLC2->setEnabled(true);
    }
}

void MainWindow::AlPresionarDesconectarPLC2() {
    if (plc2 && plc2->Connected()) plc2->Disconnect();
    ui->PB_conectar_PLC2->setEnabled(true);
    ui->PB_desconectar_PLC2->setEnabled(false);
    ui->statusbar->setStyleSheet("color: black;");
    ui->statusbar->showMessage("PLC 2 Desconectado.");
}

// =========================================================================
// EVENTO CICLICLO DEL TIMER: SE EJECUTA CADA 500ms
// =========================================================================

void MainWindow::LeerPLCsCiclicamente() {
    // Lectura automática del PLC 1
    if (plc1 && plc1->Connected()) {
        uint8_t buffer;
        int resultado = plc1->MBRead(0, 1, &buffer); // Lee el byte de marcas MB0
        if (resultado == 0) {
            ui->LBL_PLC1->setText("PLC 1 (MB0): " + QString::number(buffer));
        }
    }

    // Lectura automática del PLC 2
    if (plc2 && plc2->Connected()) {
        uint8_t buffer;
        int resultado = plc2->MBRead(0, 1, &buffer); // Lee el byte de marcas MB0
        if (resultado == 0) {
            ui->LBL_PLC2->setText("PLC 2 (MB0): " + QString::number(buffer));
        }
    }
}

// Cierre forzado de seguridad al cerrar la app
void MainWindow::cerrarConexionesSeguras() {
    if (plc1 && plc1->Connected()) plc1->Disconnect();
    if (plc2 && plc2->Connected()) plc2->Disconnect();
}
