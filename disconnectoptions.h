#ifndef DISCONNECTOPTIONS_H
#define DISCONNECTOPTIONS_H

extern "C" {
#include <MQTTAsync.h>
}

namespace linquitto {

class DisconnectOptions
{
public:
    DisconnectOptions();

    void setContext(void *context);
    void setTimeout(int timeout);
    void setOnSuccessCallback(MQTTAsync_onSuccess* callback);
    void setOnFailureCallback(MQTTAsync_onFailure* callback);
    MQTTAsync_disconnectOptions const * getRawOptions() const;


private:
    MQTTAsync_disconnectOptions m_opts;
};

}


#endif // DISCONNECTOPTIONS_H
