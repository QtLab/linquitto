#ifndef DEFAULTEVENTCALLBACK_H
#define DEFAULTEVENTCALLBACK_H

#include <QObject>

#include "eventcallback.h"

namespace linquitto {

class DefaultEventCallback : public QObject, public EventCallback
{
    Q_OBJECT
public:
    explicit DefaultEventCallback(QObject *parent = 0);

signals:
    void messageArrived(const QString &topic, const QString &message);
    void deliveryComplete(int id);
    void connectionLost(const QString &cause);

public slots:


    // EventCallback interface
public:
    void onMessageArrived(QString topic, QString message) override;
    void onDeliveryComplete(int id) override;
    void onConnectionLost(QString cause) override;
};

} // namespace linquitto


#endif // DEFAULTEVENTCALLBACK_H
