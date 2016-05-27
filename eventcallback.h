#ifndef EVENTCALLBACK_H
#define EVENTCALLBACK_H

#include <QString>

namespace linquitto {

class EventCallback
{
public:
    virtual ~EventCallback() {}

    /*!
     * \brief onMessageArrived will be called when a message/payload has arrived
     * \param topic subscribed topic
     * \param message
     *
     * Implementations should consider that it will very probably
     * run on a different thread than the UI.
     */
    virtual void onMessageArrived(QString topic, QString message) = 0;

    /*!
     * \brief onDeliveryComplete will be called when a delivery is complete
     * \param id of the message/payload
     *
     * Implementations should consider that it will very probably
     * run on a different thread than the UI.
     */
    virtual void onDeliveryComplete(int id) = 0;

    /*!
     * \brief onConnectionLost will be called when a connection got lost
     * \param cause of the loss
     *
     * Implementations should consider that it will very probably
     * run on a different thread than the UI.
     */
    virtual void onConnectionLost(QString cause) = 0;
};

}


#endif // EVENTCALLBACK_H
