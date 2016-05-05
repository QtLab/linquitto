#ifndef UNSUBSCRIBEACTIONLISTENER_H
#define UNSUBSCRIBEACTIONLISTENER_H

#include <QObject>
#include <mqtt/iaction_listener.h>

class UnsubscribeActionListener : public QObject, public mqtt::iaction_listener
{
    Q_OBJECT
public:
    explicit UnsubscribeActionListener(QObject *parent = 0);

signals:
    void failure(QString topic);
    void success(QString topic);

public slots:

    // iaction_listener interface
public:
    void on_failure(const mqtt::itoken &asyncActionToken);
    void on_success(const mqtt::itoken &asyncActionToken);
};

#endif // UNSUBSCRIBEACTIONLISTENER_H
