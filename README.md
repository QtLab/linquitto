# linquitto
I created this little project to learn how to use the eclipse paho mqtt client library on linux. Particulary the asynchronous part, since the sequential one is quite easy to grasp.

So what does this project at this stage?
You can create multiple connections. Connection name, address of the server (broker) and the port can be set.
Then you can send messages/payload to a topic or subcribe to a topic of your choosing. It uses a qos (quality of service) of 1.

To compile and run this project succesfully you need the paho c-client installed parallel to this project.
A directory structure should look similar to this:
- ...
- home/yourname/.../paho.mqtt.c
- home/yourname/.../linquetto
- ...

The paho.mqtt.c library can be checked out from https://github.com/eclipse/paho.mqtt.c
See more how to install and use this library on https://www.eclipse.org/paho/clients/c/

And you need obviously a mqtt server. On ubuntu 15.10 it is in the mosquitto package. For testing purposes it also recommended to install the mosquitto-clients package via sudo apt-get install mosquitto-clients.

My first approach in project was to use the paho.mqtt.cpp library as a layer over the c library. But after implementing some usefull functionality I discovered that the paho.mqtt.cpp library is far from complete.
Some things like interfaces to TSL and last will are not yet implemented. And the interface style leans strongly on the Java implementation for the mqtt client and tends to be a tad bit on the heavy side i.e. much code which I will probably never use. So I had to decide either to fork the paho.mqtt.cpp library and implement the missing pieces or create a own thin c++ layer over the paho.mqtt.c library. I chose the latter and will hopefully get a deeper insight into the mqtt c library, useful for a project in the future that will use this knowledge on MacOS and Swift.
