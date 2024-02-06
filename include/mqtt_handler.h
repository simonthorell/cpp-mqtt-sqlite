#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <string>
#include <mosquitto.h>
#include "sqlite_database.h"
#include "json_parser.h"

class MQTTHandler {
private:
    struct mosquitto* mosq;
    const char* host;
    int port;
    const char* cafile;
    const char* username;
    const char* password;

    JSONParser& jsonParser;
    SQLiteDatabase& db;
public:
    MQTTHandler(const char* id, const char* host, int port, const char* cafile, const char* username, const char* password, JSONParser& jsonParser, SQLiteDatabase& db);
    ~MQTTHandler();

    void connect();
    void disconnect();
    void subscribe(const char* topic);

    static void on_connect(struct mosquitto* mosq, void* obj, int reason_code);
    static void on_disconnect(struct mosquitto* mosq, void* obj, int rc);
    static void on_subscribe(struct mosquitto* mosq, void* obj, int mid, int qos_count, const int* granted_qos);
    static void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message);
};

#endif // MQTT_HANDLER_H