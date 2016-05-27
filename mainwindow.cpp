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
    ui(new Ui::MainWindow),
    m_connectActionCallback("Connect"),
    m_disconnectActionCallback("Disconnect"),
    m_publishActionCallback("Publish"),
    m_subscribeActionCallback("Subscribe"),
    m_unsubscribeActionCallback("Unsubscribe")
{
    ui->setupUi(this);

    // create and connect shortcuts:
    QShortcut *testShortcut = new QShortcut(QKeySequence(tr("Ctrl+N", "new connection")),
                                        this);
    connect(testShortcut, &QShortcut::activated, this, &MainWindow::onTestConnection);

    QShortcut *publishShortcut =
            new QShortcut(QKeySequence(tr("Ctrl+P", "publish")), this);
    connect(publishShortcut, &QShortcut::activated, this, &MainWindow::onTestPublish);

    QShortcut *subscribeShortcut =
            new QShortcut(QKeySequence(tr("Ctrl+S", "subscribe")), this);
    connect(subscribeShortcut, &QShortcut::activated, this, &MainWindow::onTestSubscribe);

    QShortcut *unsubscribeShortcut =
            new QShortcut(QKeySequence(tr("Ctrl+U", "unsubscribe")), this);
    connect(unsubscribeShortcut, &QShortcut::activated, this, &MainWindow::onTestUnsubscribe);

    // connect the menu entries:
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionNew_Connection, &QAction::triggered,
            this, &MainWindow::onCreateConnection);
    connect(ui->actionTest_Connection, &QAction::triggered,
            this, &MainWindow::onTestConnection);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::closeTab);

    // connect the buttons:
    connect(ui->createConnectionButton, &QPushButton::clicked,
            this, &MainWindow::onCreateConnection);

    connect(&m_connectActionCallback, &linquitto::DefaultActionCallback::success,
            this, &MainWindow::onSuccess);
    connect(&m_connectActionCallback, &linquitto::DefaultActionCallback::failure,
            this, &MainWindow::onFailure);
    connect(&m_eventCallback, &linquitto::DefaultEventCallback::connectionLost,
            this, &MainWindow::onConnectionLost);
    connect(&m_eventCallback, &linquitto::DefaultEventCallback::deliveryComplete,
            this, &MainWindow::onDeliveryComplete);
    connect(&m_eventCallback, &linquitto::DefaultEventCallback::messageArrived,
            this, &MainWindow::onMessageArrived);

    m_pclient.setCallback(m_eventCallback);
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

void MainWindow::onTestConnection()
{
    qDebug() << "Connect/Disconnect with a new protectable connection.";
    if(m_pclient.isConnected()) {
        qDebug() << "Disconnecting";
        linquitto::DisconnectOptions disconnOptions;
        disconnOptions.setActionCallback(&m_disconnectActionCallback);
        m_pclient.disconnect(disconnOptions);
    } else {
        qDebug() << "Connecting";
        linquitto::ConnectOptions connOptions;
        connOptions.setActionCallback(&m_connectActionCallback);
        m_pclient.connect(connOptions);
    }
}

void MainWindow::onTestPublish()
{
    qDebug() << "Publish with protectable connection.";
    if(m_pclient.isConnected()) {
        linquitto::ResponseOptions responseOptions;
        responseOptions.setActionCallback(&m_publishActionCallback);
        linquitto::Message message;
        message.setPayload("A test in publishing.");
        message.setQualityOfService(1);
        message.setRetained(false);
        m_pclient.publish("Test", message, responseOptions);
    } else {
        qDebug() << "Can't publish. Not connected!";
    }
}

void MainWindow::onTestSubscribe()
{
    qDebug() << "Subcribe with protectable connection.";
    if(m_pclient.isConnected()) {
        linquitto::ResponseOptions responseOptions;
        responseOptions.setActionCallback(&m_subscribeActionCallback);
        m_pclient.subscribe("Test", 1, responseOptions);
    }
}

void MainWindow::onTestUnsubscribe()
{
    qDebug() << "Unsubscribe with protectable connection.";
    if(m_pclient.isConnected()) {
        linquitto::ResponseOptions responseOptions;
        responseOptions.setActionCallback(&m_unsubscribeActionCallback);
        m_pclient.unsubscribe("Test", responseOptions);
    }
}

void MainWindow::closeTab(int index)
{
    if(ui->tabWidget->currentIndex() ==  index) {
        ui->tabWidget->currentWidget()->deleteLater();
        ui->tabWidget->removeTab(index);
    }
}

void MainWindow::onSuccess()
{
    addLog("Connected.");
}

void MainWindow::onFailure(int errorCode, const QString &errorMessage)
{
    addLog("Connecting failed: [" + QString::number(errorCode) + "] " + errorMessage);
}

void MainWindow::onMessageArrived(const QString &topic, const QString &message)
{
    addLog("Message arrived: [" + topic + "] " + message);
}

void MainWindow::onDeliveryComplete(int id)
{
    addLog("Delivery for message " + QString::number(id) + " complete.");
}

void MainWindow::onConnectionLost(const QString &cause)
{
    addLog("Connection lost: " + cause);
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
