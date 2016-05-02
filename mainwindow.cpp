#include "mainwindow.h"
#include "ui_mainwindow.h"

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENTID("SyncPublisher");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_connected(false),
    m_client(new mqtt::client(ADDRESS, CLIENTID, 0))
{
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(switchConnection()));
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
    } else {
        ui->connectButton->setText("connect");
    }
}
