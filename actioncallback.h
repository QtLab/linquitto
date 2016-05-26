#ifndef ACTIONCALLBACK_H
#define ACTIONCALLBACK_H

#include <QString>

namespace linquitto {

class ActionCallback
{
public:
    ActionCallback();
    virtual ~ActionCallback() {}

    /*!
     * \brief onSuccess will be called when an action was successful.
     *
     * Implementations should consider that it will very probably
     * run on a different thread than the UI.
     */
    virtual void onSuccess() = 0;

    /*!
     * \brief onFailure will be called when an action fails.
     * \param errorCode
     * \param errorMessage
     *
     * Implementations should consider that it will very probably
     * run on a different thread than the UI thread.
     */
    virtual void onFailure(int errorCode, QString errorMessage) = 0;
};

} // namespace linquitto

#endif // ACTIONCALLBACK_H
