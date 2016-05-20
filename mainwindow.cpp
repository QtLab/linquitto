#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>

#include "mqtt/async_client.h"

#include "makeunique.h"
#include "createconnectiondialog.h"

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
//    m_connections[CLIENTID] =
//            linquitto::make_unique<AsyncConnection>(
//                linquitto::make_unique<mqtt::async_client>(ADDRESS, CLIENTID));


    // connect the menu entries:
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionNew_Connection, &QAction::triggered,
            this, &MainWindow::onCreateConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchConnection()
{
    if(m_connections[actualConnectionName]->isConnectedWithServer()) {
        m_connections[actualConnectionName]->disconnectFromServer();
    } else {
        m_connections[actualConnectionName]->connectWithServer();
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
    m_connections[actualConnectionName]->subscribeToTopic(topic);
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

    m_connections[actualConnectionName]->unsubscribeFromTopic(topic);
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
    m_connections[actualConnectionName]->publishMessage(topic, message);
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

    if( m_connections.find(name) == m_connections.end() ){
        m_connections[name] =
                linquitto::make_unique<AsyncConnection>(
                    linquitto::make_unique<mqtt::async_client>(
                        brokerString.toStdString(), name.toStdString()));
        qDebug() << "New connection created.";
        actualConnectionName = name;
        connectSignals(name);
    } else {
        qDebug() << "No connection created. Name" << name << "already exists!";
    }

}

void MainWindow::connectSignals(QString name)
{
    // connect the buttons
    connect(ui->connectButton, &QPushButton::clicked,
            this, &MainWindow::switchConnection);
    connect(ui->publishButton, &QPushButton::clicked,
            this, &MainWindow::onPublish);
    connect(ui->subscribeButton, &QPushButton::clicked,
            this, &MainWindow::onSubscribe);
    connect(ui->unsubscribeButton, &QPushButton::clicked,
            this, &MainWindow::onUnsubscribe);

    // connect with signals from connection object
    // get a non owning pointer from the connection map for the connect operation:
    AsyncConnection *pConnection = m_connections[name].get();
    // and now connect:
    connect(pConnection, &AsyncConnection::connected,
            this, &MainWindow::connectionEstablished);
    connect(pConnection, &AsyncConnection::disconnected,
            this, &MainWindow::disconnected);
    connect(pConnection, &AsyncConnection::published,
            this, &MainWindow::connectionHasPublished);
    connect(pConnection, &AsyncConnection::subscribed,
            this, &MainWindow::connectionHasSubscribed);
    connect(pConnection, &AsyncConnection::unsubscribed,
            this, &MainWindow::connectionHasUnsubscribed);
    connect(pConnection, &AsyncConnection::messageArrived,
            this, &MainWindow::messageArrived);
    connect(pConnection, &AsyncConnection::connectionLost,
            this, &MainWindow::connectionLost);
    ui->connectButton->setEnabled(true);
}
