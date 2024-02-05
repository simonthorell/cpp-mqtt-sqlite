#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <string>
#include <mqtt/client.h> // Paho MQTT C++ client

class MQTTHandler {
private:
    mqtt::client client;
    std::string topic;
    int QOS;
    class callback : public virtual mqtt::callback {
    public:
        void message_arrived(mqtt::const_message_ptr msg) override;
    };
    callback cb; // Instance of the callback class

public:
    MQTTHandler(const std::string& serverURI, const std::string& clientId, const std::string& topic, int QOS);
    void connect();
    void subscribe();
    void disconnect();
};


#endif // MQTT_HANDLER_H