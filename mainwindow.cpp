#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>
#include <QUrl>

#include "mqtt/async_client.h"

#include "makeunique.h"
#include "createconnectiondialog.h"
#include "connectioncontent.h"

#include <QDebug>

/*!
 * \brief MainWindow::MainWindow creates the instance, builds the ui and connects signals/slots
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // connect the menu entries:
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionNew_Connection, &QAction::triggered,
            this, &MainWindow::onCreateConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addLog(QString message)
{
    ui->logList->addItem(message);
    // show the last entry:
    ui->logList->scrollToBottom();
}

void MainWindow::onCreateConnection()
{
    qDebug() << "Create a new connection.";
    CreateConnectionDialog dialog(this);
    int result = dialog.exec();
    qDebug() << "result=" << result;
    qDebug() << "name=" << dialog.getName();
    qDebug() << "broker=" << dialog.getBroker();
    qDebug() << "port=" << dialog.getPort();
    createConnection(dialog.getName(), dialog.getBroker(), dialog.getPort());
}

void MainWindow::createConnection(QString name, QString broker, int port)
{
    if(name.isEmpty() || broker.isEmpty()) {
        return;
    }
    QString brokerString = "tcp://" + broker + ":" + QString::number(port);
    QUrl url(brokerString);
    if(url.isValid()) {
        qDebug() << "Valid url.";
    } else {
        qDebug() << "Invalid url!";
        return;
    }

    if(isUniqueTabName(name)) {
        ConnectionContent *content = new ConnectionContent(brokerString, name);
        int tabIndex = ui->tabWidget->addTab(content, name);
        connect(content, &ConnectionContent::log, this, &MainWindow::addLog);
        ui->tabWidget->setCurrentIndex(tabIndex);
    } else {
        qDebug() << "MainWindow::createConnection: " << name << " is not unique!";
    }

}

bool MainWindow::isUniqueTabName(QString name)
{
    int tabs = ui->tabWidget->count();
    for(int i=0; i<tabs; ++i) {
        if(name == ui->tabWidget->tabText(i)) {
            return false;
        }
    }
    return true;
}
