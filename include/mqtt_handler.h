#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <string>
#include <mqtt/client.h>       // Paho MQTT C++ client
#include "sqlite_database.h"   // SQLiteDatabase class
#include "json_parser.h"       // JSON parser class

class MQTTHandler {
private:
    mqtt::client client;
    std::string topic;
    int QOS;
    SQLiteDatabase& db;
    JSONParser& jsonParser;

    class callback : public virtual mqtt::callback {
        MQTTHandler& handler;
    public:
        callback(MQTTHandler& handler) : handler(handler) {}
        void message_arrived(mqtt::const_message_ptr msg) override;
    };
    callback cb;

public:
    MQTTHandler(const std::string& serverURI, const std::string& clientId, const std::string& topic, int QOS, SQLiteDatabase& db, JSONParser& jsonParser);
    void connect();
    void subscribe();
    void disconnect();
    void handleMessage(mqtt::const_message_ptr msg);
};

#endif // MQTT_HANDLER_H