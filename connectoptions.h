#ifndef CONNECTOPTIONS_H
#define CONNECTOPTIONS_H

extern "C" {
    #include <MQTTAsync.h>
}

namespace linquitto {

class ConnectOptions
{
public:
    ConnectOptions();
     MQTTAsync_connectOptions *getOptions();

private:
    MQTTAsync_connectOptions m_opts;

};

} // end of namespace linquitto


#endif // CONNECTOPTIONS_H
