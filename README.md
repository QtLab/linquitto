# linquitto
I created this little project to learn how to use the eclipse paho mqtt client libraries on linux. Particulary the asynchronous part, since the sequential one is quite easy to grasp.

So what does this project at this stage?
It simply connects with a mqtt server on localhost (port 1883). You can send then messages/payload to a topic or subcribe to a topic of your choosing. It uses a qos (quality of service) of 1.

To compile and run this project succesfully you need the paho c-client AND the c++-client libs installed parallel to this project.
A directory structure should look similar to this:
- ...
- home/yourname/.../paho.mqtt.c
- home/yourname/.../paho.mqtt.cpp
- home/yourname/.../linquetto
- ...

Both paho libs can be checked out from the appropriate repositories on github.
i.e.: https://github.com/eclipse/paho.mqtt.c and https://github.com/eclipse/paho.mqtt.cpp.
See more how to install and use these libs on https://www.eclipse.org/paho/clients/c/ AND http://www.eclipse.org/paho/clients/cpp/

And you need obviously a mqtt server. On ubuntu 15.10 it is in the mosquitto package. For testing purposes it also recommended to install the mosquitto-clients package via sudo apt-get install mosquitto-clients.
