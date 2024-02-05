#include "mqtt_handler.h"
#include "salt_generator.h"
#include "hash_generator.h"
#include <iostream>

MQTTHandler::MQTTHandler(const std::string& serverURI, const std::string& clientId, const std::string& topic, int QOS, SQLiteDatabase& dbRef, JSONParser& jsonParserRef)
    : client(serverURI, clientId), topic(topic), QOS(QOS), db(dbRef), jsonParser(jsonParserRef), cb(*this) {
    client.set_callback(cb);
}

void MQTTHandler::connect() {
    try {
        std::cout << "Connecting to the MQTT server..." << std::endl;
        client.connect();
        std::cout << "Connected!" << std::endl;
    } catch (const mqtt::exception& exc) {
        std::cerr << "Connection failed: " << exc.what() << std::endl;
        exit(1);
    }
}

void MQTTHandler::subscribe() {
    try {
        std::cout << "Subscribing to topic: " << topic << std::endl;
        client.subscribe(topic, QOS);
        std::cout << "Subscribed!" << std::endl;
    } catch (const mqtt::exception& exc) {
        std::cerr << "Subscription failed: " << exc.what() << std::endl;
        exit(1);
    }
}

void MQTTHandler::disconnect() {
    try {
        std::cout << "Disconnecting from the MQTT server..." << std::endl;
        client.disconnect();
        std::cout << "Disconnected!" << std::endl;
    } catch (const mqtt::exception& exc) {
        std::cerr << "Disconnection failed: " << exc.what() << std::endl;
        exit(1);
    }
}

void MQTTHandler::handleMessage(mqtt::const_message_ptr msg) {
    std::cout << "Message arrived: " << msg->get_topic() << std::endl;
    std::string payload = msg->to_string();

    std::map<std::string, std::string> decryptedData = jsonParser.parseAndDecrypt(payload, "YourEncryptionKeyHere");

    // Process decrypted data (store in database, etc.)
    std::string email = decryptedData["email"];        // std::string email = "user@example.com";
    std::string password = decryptedData["password"];  // std::string password = "password";

    // Generate a secure, random salt for each password
    SaltGenerator saltGenerator;
    std::string salt = saltGenerator.generateSalt(12);

    // Salt & Hash the password
    HashGenerator hashGenerator;
    std::string saltedPassword = password + salt;
    std::string hashedPassword = hashGenerator.computeSHA256(saltedPassword);

    // Insert user data into the users table
    if (!db.insert("users", "email, password, salt", "'" + email + "', '" + hashedPassword + "', '" + salt + "'")) {
        std::cerr << "Data insertion failed!" << std::endl;
        return;
    }
}

void MQTTHandler::callback::message_arrived(mqtt::const_message_ptr msg) {
    handler.handleMessage(msg);
}