#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>

#include "mqtt/async_client.h"

#include "makeunique.h"

#include <QDebug>

namespace linquitto {
const QString emptyFill("...");
}

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENTID("AsyncPublisher");

/*!
 * \brief MainWindow::MainWindow creates the instance, builds the ui and connects signals/slots
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_connections[CLIENTID] =
            linquitto::make_unique<AsyncConnection>(
                linquitto::make_unique<mqtt::async_client>(ADDRESS, CLIENTID));


    // connect the buttons
    // TODO: change the SIGNAL/SLOT macro calls to function pointer,
    //       since the compiler can now better check if the method signatures are compatible
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::switchConnection);
    //connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(switchConnection()));
    connect(ui->publishButton, SIGNAL(clicked()), this, SLOT(onPublish()));
    connect(ui->subscribeButton, SIGNAL(clicked()), this, SLOT(onSubscribe()));
    connect(ui->unsubscribeButton, SIGNAL(clicked()), this, SLOT(onUnsubscribe()));

    // connect with signals from connection object
    AsyncConnection *pConnection = m_connections[CLIENTID].get();
    connect(pConnection, SIGNAL(connected()), this, SLOT(connectionEstablished()));
    connect(pConnection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(pConnection, SIGNAL(published()), this, SLOT(connectionHasPublished()));
    connect(pConnection, SIGNAL(subscribed()), this, SLOT(connectionHasSubscribed()));
    connect(pConnection, SIGNAL(unsubscribed(QString)),
            this, SLOT(connectionHasUnsubscribed(QString)));
    connect(pConnection, SIGNAL(messageArrived(QString,QString)),
            this, SLOT(messageArrived(QString,QString)));
    connect(pConnection, SIGNAL(connectionLost(QString)),
            this, SLOT(connectionLost(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchConnection()
{
    if(m_connections[CLIENTID]->isConnectedWithServer()) {
        m_connections[CLIENTID]->disconnectFromServer();
    } else {
        m_connections[CLIENTID]->connectWithServer();
    }
}

void MainWindow::addLog(QString message)
{
    ui->logList->addItem(message);
}

void MainWindow::connectionEstablished()
{
    ui->connectButton->setText("disconnect");
    ui->subscribeButton->setEnabled(true);
    ui->publishButton->setEnabled(true);
    ui->logList->addItem("Connected with Server");
}

void MainWindow::disconnected()
{
    ui->connectButton->setText("connect");
    ui->subscribeButton->setEnabled(false);
    ui->publishButton->setEnabled(false);
    ui->topicCombo->clear();
    ui->unsubscribeButton->setEnabled(false);
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
    const QString topic = ui->subscribeTopicEdit->toPlainText();
    ui->logList->addItem("Subscribed to topic " + topic);
    qDebug() << "MainWindow::connectionHasSubscribed: " << topic;
    ui->topicCombo->addItem(topic);
    ui->unsubscribeButton->setEnabled(true);
}

void MainWindow::connectionHasUnsubscribed(QString topic)
{
    ui->logList->addItem("Unsubscribed from topic " + topic);
    int index = ui->topicCombo->findText(topic);
    if(index > -1) {
        ui->topicCombo->removeItem(index);
    }
    if(ui->topicCombo->count() == 0) {
        // When there is nothing to unsubscribe from, disable the button.
        ui->unsubscribeButton->setEnabled(false);
    }
}

void MainWindow::connectionLost(QString cause)
{
    ui->logList->addItem("Connection lost: " + cause);
    ui->connectButton->setText("connect");
    ui->subscribeButton->setEnabled(false);
    ui->publishButton->setEnabled(false);
}

void MainWindow::messageArrived(QString topic, QString message)
{
    ui->messageList->addItem("(" + topic + ") " + message);
}

void MainWindow::onSubscribe()
{
    qDebug() << "subscribe" << "clicked";
    const QString topic = ui->subscribeTopicEdit->toPlainText();

    // test if topic is not filled
    if(topic.isEmpty() || topic == linquitto::emptyFill) {
        qDebug() << "MainWindow::onSubscribe: topic is empty!";
        ui->topicEdit->setPlainText(linquitto::emptyFill);
        return;
    }

    // subscribe only when topic is filled:
    m_connections[CLIENTID]->subscribeToTopic(topic);
}

void MainWindow::onUnsubscribe()
{
    qDebug() << "unsubscribe" << "clicked";

    if(ui->topicCombo->count() == 0) {
        // TODO: throw appropriate exception
        qDebug() << "Nothing to unsubscribe from, this should not happen!";
        return;
    }

    // Get the current item
    const QString topic = ui->topicCombo->currentText();
    if(topic.isEmpty()) {
        qDebug() << "topic for unsubcribe is empty, should not happen!";
        // TODO: throw appropriate exception, cause this should really not happen.
        return;
    }

    m_connections[CLIENTID]->unsubscribeFromTopic(topic);
}

void MainWindow::onPublish()
{
    qDebug() << "publish" << "clicked.";
    const QString topic = ui->topicEdit->toPlainText();
    const QString message = ui->messageEdit->toPlainText();


    // test if topic and message are filled:
    if(topic.isEmpty() || topic == linquitto::emptyFill) {
        qDebug() << "MainWindow::onPublish: topic is empty!";
        ui->topicEdit->setPlainText(linquitto::emptyFill);
        return;
    }

    if(message.isEmpty() || message == linquitto::emptyFill) {
        qDebug() << "MainWindow::onPublish: message is empty!";
        ui->messageEdit->setPlainText(linquitto::emptyFill);
        return;
    }

    // publish only when topic and message are filled
    m_connections[CLIENTID]->publishMessage(topic, message);
}
