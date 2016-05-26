#ifndef DISCONNECTOPTIONS_H
#define DISCONNECTOPTIONS_H

extern "C" {
#include <MQTTAsync.h>
}

namespace linquitto {

class ActionCallback;

class DisconnectOptions
{
public:
    DisconnectOptions();

    void setTimeout(int timeout);
    void setActionCallback(ActionCallback *callback);
    MQTTAsync_disconnectOptions const * getRawOptions() const;

private:
    MQTTAsync_disconnectOptions m_opts;
};

}


#endif // DISCONNECTOPTIONS_H
