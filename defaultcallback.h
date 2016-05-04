#ifndef DEFAULTCALLBACK_H
#define DEFAULTCALLBACK_H

#include <QObject>

#include <mqtt/callback.h>

class DefaultCallback : public QObject, public mqtt::callback
{
    Q_OBJECT
public:
    explicit DefaultCallback(QObject *parent = 0);

signals:
    void connectionLost(QString cause);
    void messageArrived(QString topic, QString message);
    void deliveryComplete();

public slots:

    // callback interface
public:
    void connection_lost(const std::__cxx11::string &cause);
    void message_arrived(const std::__cxx11::string &topic, mqtt::message_ptr msg);
    void delivery_complete(mqtt::idelivery_token_ptr tok);
};

#endif // DEFAULTCALLBACK_H
