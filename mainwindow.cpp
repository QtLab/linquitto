#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_connected(false),
    m_connection()
{
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(switchConnection()));
    connect(&m_connection, SIGNAL(logIt(QString)), this, SLOT(addLog(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchConnection()
{
    m_connected = !m_connected; // switch
    if(m_connected) {
        ui->connectButton->setText("disconnect");
        m_connection.connectWithServer();
    } else {
        ui->connectButton->setText("connect");
        m_connection.disconnectFromServer();
    }
}

void MainWindow::addLog(QString message)
{
    ui->logList->addItem(message);
}
