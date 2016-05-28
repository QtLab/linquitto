#include "connectioncontent.h"
#include "ui_connectioncontent.h"
#include "makeunique.h"

#include <QDebug>

using namespace linquitto;

ConnectionContent::ConnectionContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionContent),
    m_connection(make_unique<ProtectableAsyncClient>(
                     "tcp://localhost:1883", "Default"))
{
    ui->setupUi(this);
    ui->brokerLabel->setText("tcp://localhost:1883");
    connectSignals();
}

ConnectionContent::ConnectionContent(QString &brokerUrl,
                                     QString &clientName,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionContent),
    m_connection(make_unique<ProtectableAsyncClient>(brokerUrl, clientName))
{
    ui->setupUi(this);
    ui->brokerLabel->setText(brokerUrl);
    connectSignals();
}

ConnectionContent::~ConnectionContent()
{
    delete ui;
}

void ConnectionContent::onConnect()
{
    if(!m_connection.isConnectedWithServer()) {
        m_connection.connectWithServer();
    }
}

void ConnectionContent::onDisconnect()
{
    if(m_connection.isConnectedWithServer()) {
        m_connection.disconnectFromServer();
    }
}

void ConnectionContent::onPublish()
{
    qDebug() << "publish" << "clicked.";

    const QString topic = ui->publishTopicEdit->text();
    const QString message = ui->publishPayloadEdit->text();


    // test if topic and message are filled:
    if(topic.isEmpty()) {
        qDebug() << "ConnectionContent::onPublish: topic is empty!";
        return;
    }

    if(message.isEmpty()) {
        qDebug() << "ConnectionContent::onPublish: message is empty!";
        return;
    }

    // publish only when topic and message are filled
    m_connection.publishMessage(topic, message);
}

void ConnectionContent::onSubscribe()
{
    // subscribe only when topic is filled and not allready subscribed to

    qDebug() << "subscribe" << "clicked";
    const QString topic = ui->subscribeTopicEdit->text();

    // test if topic is not filled
    if(topic.isEmpty()) {
        qDebug() << "ConnectionContent::onSubscribe: topic is empty!";
        return;
    }

    if(notAlreadySubscribed(topic)) {
        m_connection.subscribeToTopic(topic);
    } else {
        qDebug() << "ConnectionContent::onSubscribe: already subscribed to topic!";
    }
}

void ConnectionContent::onUnsubscribe()
{
    qDebug() << "unsubscribe" << "clicked";

    if(ui->subscriptionsCombo->count() == 0) {
        // TODO: throw appropriate exception
        qDebug() << "Nothing to unsubscribe from, this should not happen!";
        return;
    }

    // Get the current item
    const QString topic = ui->subscriptionsCombo->currentText();
    if(topic.isEmpty()) {
        qDebug() << "topic for unsubcribe is empty, should not happen!";
        // TODO: throw appropriate exception, cause this should really not happen.
        return;
    }

    m_connection.unsubscribeFromTopic(topic);
}

void ConnectionContent::connectionEstablished()
{
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    ui->subscribeButton->setEnabled(true);
    ui->publishButton->setEnabled(true);
    emit log("Connected with Server.");
}

void ConnectionContent::onConnectFailed(int errorCode, const QString &errorMessage)
{
    QString logMessage = QString("[%1] Connecting with %2 failed: (%3) %4")
            .arg(m_connection.getClientId())
            .arg(m_connection.getBrokerUrl())
            .arg(errorCode)
            .arg(errorMessage);
    emit log(logMessage);
}

void ConnectionContent::disconnected()
{
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->subscribeButton->setEnabled(false);
    ui->unsubscribeButton->setEnabled(false);
    ui->publishButton->setEnabled(false);
    ui->subscriptionsCombo->clear();
    ui->subscribeTopicEdit->clear();
    ui->publishTopicEdit->clear();
    ui->publishPayloadEdit->clear();
    emit log("Disconnected from Server.");

}

void ConnectionContent::onDisconnectFailed(int errorCode, const QString &errorMessage)
{
    QString logMessage = QString("[%1] Disconnecting from %2 failed: (%3) %4")
            .arg(m_connection.getClientId())
            .arg(m_connection.getBrokerUrl())
            .arg(errorCode)
            .arg(errorMessage);
    emit log(logMessage);
}

void ConnectionContent::connectionHasPublished()
{
    const QString topic = ui->publishTopicEdit->text();
    const QString message = ui->publishPayloadEdit->text();
    emit log(message + " was published on " + topic);
}

void ConnectionContent::onPublishFailed(int errorCode, const QString &errorMessage)
{
    QString logMessage = QString("[%1] Publishing to %2 failed: (%3) %4")
            .arg(m_connection.getClientId())
            .arg(m_connection.getBrokerUrl())
            .arg(errorCode)
            .arg(errorMessage);
    emit log(logMessage);
}

void ConnectionContent::connectionHasSubscribed(const QString &topic)
{
    emit log("Subscribed to topic " + topic);
    qDebug() << "ConnectionContent::connectionHasSubscribed: " << topic;
    ui->subscriptionsCombo->addItem(topic);
    ui->unsubscribeButton->setEnabled(true);
}

void ConnectionContent::onSubscribeFailed(const QString &topic, int errorCode, const QString &errorMessage)
{
    QString logMessage = QString("[%1] Subscribing to \"%2\" on %3 failed: (%4) %5")
            .arg(m_connection.getClientId())
            .arg(m_connection.getBrokerUrl())
            .arg(topic)
            .arg(errorCode)
            .arg(errorMessage);
    emit log(logMessage);
}

void ConnectionContent::connectionHasUnsubscribed(const QString &topic)
{
    emit log("Unsubscribed from topic " + topic);
    int index = ui->subscriptionsCombo->findText(topic, Qt::MatchExactly);
    if(index > -1) {
        ui->subscriptionsCombo->removeItem(index);
    }
    if(ui->subscriptionsCombo->count() == 0) {
        // When there is nothing to unsubscribe from, disable the button.
        ui->unsubscribeButton->setEnabled(false);
    }
}

void ConnectionContent::onUnsubscribeFailed(const QString &topic, int errorCode, const QString &errorMessage)
{
    QString logMessage = QString("[%1] Unsubscribing from \"%2\" on %3 failed: (%4) %5")
            .arg(m_connection.getClientId())
            .arg(m_connection.getBrokerUrl())
            .arg(topic)
            .arg(errorCode)
            .arg(errorMessage);
    emit log(logMessage);
}

void ConnectionContent::connectionLost(QString cause)
{
    emit log("Connection lost: " + cause);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->subscribeButton->setEnabled(false);
    ui->publishButton->setEnabled(false);
}

void ConnectionContent::messageArrived(QString topic, QString message)
{
    ui->subscriptionMessages->addItem("[" + topic + "]: " + message);
}

void ConnectionContent::connectSignals()
{
    // connect the buttons
    connect(ui->connectButton, &QPushButton::clicked,
            this, &ConnectionContent::onConnect);
    connect(ui->disconnectButton, &QPushButton::clicked,
            this, &ConnectionContent::onDisconnect);
    connect(ui->publishButton, &QPushButton::clicked,
            this, &ConnectionContent::onPublish);
    connect(ui->subscribeButton, &QPushButton::clicked,
            this, &ConnectionContent::onSubscribe);
    connect(ui->unsubscribeButton, &QPushButton::clicked,
            this, &ConnectionContent::onUnsubscribe);

    // and now connect with the connection object:
    connect(&m_connection, &AsyncConnection::connected,
            this, &ConnectionContent::connectionEstablished);
    connect(&m_connection, &AsyncConnection::connectFailed,
            this, &ConnectionContent::onConnectFailed);
    connect(&m_connection, &AsyncConnection::disconnected,
            this, &ConnectionContent::disconnected);
    connect(&m_connection, &AsyncConnection::disconnectFailed,
            this, &ConnectionContent::onDisconnectFailed);
    connect(&m_connection, &AsyncConnection::published,
            this, &ConnectionContent::connectionHasPublished);
    connect(&m_connection, &AsyncConnection::subscribed,
            this, &ConnectionContent::connectionHasSubscribed);
    connect(&m_connection, &AsyncConnection::unsubscribed,
            this, &ConnectionContent::connectionHasUnsubscribed);
    connect(&m_connection, &AsyncConnection::messageArrived,
            this, &ConnectionContent::messageArrived);
    connect(&m_connection, &AsyncConnection::connectionLost,
            this, &ConnectionContent::connectionLost);
}

bool ConnectionContent::notAlreadySubscribed(const QString &topic) const
{
    return ui->subscriptionsCombo->findText(topic, Qt::MatchExactly) == -1;
}
