#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

#include "mqtt/callback.h"
#include "mqtt/client.h"

class Connection : public QObject, public mqtt::callback
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);
    ~Connection();

signals:
    void connectionLost(QString cause);
    void messageArrived(QString topic, QString message);
    void deliveryComplete(QString topic);
    void logIt(QString message);

public slots:
    void connectWithServer();
    void disconnectFromServer();
    void sendMessageToServer(QString message);

private:
    mqtt::client m_client;

    // callback interface
public:
    void connection_lost(const std::__cxx11::string &cause);
    void message_arrived(const std::__cxx11::string &topic, mqtt::message_ptr msg);
    void delivery_complete(mqtt::idelivery_token_ptr tok);
};

#endif // CONNECTION_H
