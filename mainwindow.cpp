#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>

#include "mqtt/async_client.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_connected(false),
    m_connection()
{
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(switchConnection()));
    connect(&m_connection, SIGNAL(connected()), this, SLOT(connectionEstablished()));
    connect(&m_connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchConnection()
{
    m_connected = !m_connected; // switch
    if(m_connection.isConnectedWithServer()) {
        m_connection.disconnectFromServer();
    } else {
        m_connection.connectWithServer();
    }
}

void MainWindow::addLog(QString message)
{
    ui->logList->addItem(message);
}

void MainWindow::connectionEstablished()
{
    ui->connectButton->setText("disconnect");
    ui->logList->addItem("Connected with Server");
}

void MainWindow::disconnected()
{
    ui->connectButton->setText("connect");
    ui->logList->addItem("Disconnected from Server.");
}
