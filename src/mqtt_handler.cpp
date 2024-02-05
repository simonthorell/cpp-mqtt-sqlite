#include "mqtt_handler.h"
#include <iostream>

MQTTHandler::MQTTHandler(const std::string& serverURI, const std::string& clientId, const std::string& topic, int QOS)
    : client(serverURI, clientId), topic(topic), QOS(QOS) {
    client.set_callback(cb);
}

void MQTTHandler::connect() {
    try {
        std::cout << "Connecting to the MQTT server..." << std::endl;
        client.connect();
        std::cout << "Connected!" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Connection failed: " << exc.what() << std::endl;
        exit(1);
    }
}

void MQTTHandler::subscribe() {
    try {
        std::cout << "Subscribing to topic: " << topic << std::endl;
        client.subscribe(topic, QOS);
        std::cout << "Subscribed!" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Subscription failed: " << exc.what() << std::endl;
        exit(1);
    }
}

void MQTTHandler::disconnect() {
    try {
        std::cout << "Disconnecting from the MQTT server..." << std::endl;
        client.disconnect();
        std::cout << "Disconnected!" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Disconnection failed: " << exc.what() << std::endl;
        exit(1);
    }
}

void MQTTHandler::callback::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "Message arrived: " << msg->get_topic() << ": " << msg->to_string() << std::endl;
}
