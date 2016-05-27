#ifndef EVENTCALLBACK_H
#define EVENTCALLBACK_H

#include <QString>

namespace linquitto {

class EventCallback
{
public:
    virtual ~EventCallback() {}

    virtual void onMessageArrived(QString topic, QString message) = 0;
    virtual void onDeliveryComplete(int id) = 0;
    virtual void onConnectionLost(QString cause) = 0;
};

}


#endif // EVENTCALLBACK_H
