#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>
#include <QUrl>
#include <QShortcut>

#include "mqtt/async_client.h"

#include "makeunique.h"
#include "createconnectiondialog.h"
#include "connectioncontent.h"
#include "protectableasyncclient.h"
#include "connectoptions.h"
#include "responseoptions.h"
#include "message.h"

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

    // create and connect shortcuts:
    QShortcut *quitShortcut =
            new QShortcut(QKeySequence(tr("Ctrl+Q", "Quit")), this);
    connect(quitShortcut, &QShortcut::activated, this, &MainWindow::close);

    // connect the menu entries:
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionNew_Connection, &QAction::triggered,
            this, &MainWindow::onCreateConnection);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::closeTab);

    // connect the buttons:
    connect(ui->createConnectionButton, &QPushButton::clicked,
            this, &MainWindow::onCreateConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::staticOnConnectSuccess(void *context, MQTTAsync_successData *data)
{
    if(context != nullptr) {
        QString *str = static_cast<QString*>(context);
        qDebug() << *str;
    } else {
        qDebug() << "context was empty.";
    }
    qDebug() << "--> Successfull connected to" << data->alt.connect.serverURI;
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

void MainWindow::closeTab(int index)
{
    if(ui->tabWidget->currentIndex() ==  index) {
        ui->tabWidget->currentWidget()->deleteLater();
        ui->tabWidget->removeTab(index);
    }
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
        content->setObjectName(name + "_ConnectionContent");
        int tabIndex = ui->tabWidget->addTab(content, name);
        connect(content, &ConnectionContent::log, this, &MainWindow::addLog);
        ui->tabWidget->setCurrentIndex(tabIndex);
    } else {
        addLog("Connection name \"" + name + "\" is not unique!");
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
