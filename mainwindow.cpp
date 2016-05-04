#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>

#include "mqtt/async_client.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_connection()
{
    ui->setupUi(this);

    // connect the buttons
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(switchConnection()));
    connect(ui->publishButton, SIGNAL(clicked()), this, SLOT(onPublish()));
    connect(ui->subscribeButton, SIGNAL(clicked()), this, SLOT(onSubscribe()));

    // connect with signals from connection object
    connect(&m_connection, SIGNAL(connected()), this, SLOT(connectionEstablished()));
    connect(&m_connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(&m_connection, SIGNAL(published()), this, SLOT(connectionHasPublished()));
    connect(&m_connection, SIGNAL(subscribed()), this, SLOT(connectionHasSubscribed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchConnection()
{
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

void MainWindow::connectionHasPublished()
{
    const QString topic = ui->topicEdit->toPlainText();
    const QString message = ui->messageEdit->toPlainText();
    ui->logList->addItem(message + " was published on " + topic);
}

void MainWindow::connectionHasSubscribed()
{
    const QString topic = ui->topicEdit->toPlainText();
    ui->logList->addItem("Subscribed to topic " + topic);
}

void MainWindow::onSubscribe()
{
    qDebug() << "subscribe" << "clicked";
}

void MainWindow::onPublish()
{
    qDebug() << "publish" << "clicked.";
    const QString topic = ui->topicEdit->toPlainText();
    const QString message = ui->messageEdit->toPlainText();
    const QString emptyFill("...");

    // test if topic and message are filled:
    if(topic.isEmpty() || topic == emptyFill) {
        qDebug() << "MainWindow::onPublish: topic is empty!";
        ui->topicEdit->setPlainText(emptyFill);
        return;
    }

    if(message.isEmpty() || message == emptyFill) {
        qDebug() << "MainWindow::onPublish: message is empty!";
        ui->messageEdit->setPlainText(emptyFill);
        return;
    }

    // publish only when topic and message are filled
    m_connection.publishMessage(topic, message);
}
