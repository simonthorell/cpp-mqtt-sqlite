#include "mqtt_handler.h"
#include <iostream>
#include "config.h"
#include "hash_generator.h"
#include "salt_generator.h"

//==============================================================================
// Constructor and destructor
//==============================================================================

MQTTHandler::MQTTHandler(const char* id, const char* host, int port, const char* cafile, const char* username, const char* password, JSONParser& jsonParser, SQLiteDatabase& db)
    : host(host), port(port), cafile(cafile), username(username), password(password), jsonParser(jsonParser), db(db) {
    mosquitto_lib_init();
    mosq = mosquitto_new(id, true, this);

    // Set TLS options
    mosquitto_tls_set(mosq, cafile, NULL, NULL, NULL, NULL);
    
    // Set username and password
    if (username && password) {
        mosquitto_username_pw_set(mosq, username, password);
    }

    // Assign the callback functions
    mosquitto_connect_callback_set(mosq, MQTTHandler::on_connect);
    mosquitto_disconnect_callback_set(mosq, MQTTHandler::on_disconnect);
    mosquitto_message_callback_set(mosq, MQTTHandler::on_message);
    mosquitto_subscribe_callback_set(mosq, MQTTHandler::on_subscribe);
}

MQTTHandler::~MQTTHandler() {
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

//==============================================================================
// Public member functions
//==============================================================================

void MQTTHandler::connect() {
    int rc = mosquitto_connect(mosq, host, port, 60);
    std::cout << "Connecting to MQTT broker..." << std::endl;
    if (rc) {
        // Handle failed connect
    }
    mosquitto_loop_start(mosq);
}

void MQTTHandler::disconnect() {
    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, false);
}

void MQTTHandler::subscribe(const char* topic) {
    int rc = mosquitto_subscribe(mosq, NULL, topic, 0);
    if (rc) {
        // Handle failed subscribe
    }
}

//==============================================================================
// Static callback functions
//==============================================================================

void MQTTHandler::on_connect(struct mosquitto* mosq, void* obj, int reason_code) {
    // Cast obj back to MQTTHandler to access class members
    MQTTHandler* handler = static_cast<MQTTHandler*>(obj);

    // Static members are used to access the non-static members
    JSONParser& jsonParser = handler->jsonParser;
    SQLiteDatabase& db = handler->db;

    // Handle the on_connect event
    if (reason_code == 0) {
        // Connection successful
        std::cout << "Connected to MQTT broker." << std::endl;
        // Add your logic here for a successful connection
    } else {
        // Connection failed
        std::cerr << "Failed to connect to MQTT broker. Reason code: " << reason_code << std::endl;
        // Add your error handling or retry logic here
    }
}

void MQTTHandler::on_disconnect(struct mosquitto* mosq, void* obj, int rc) {
    // Cast obj back to MQTTHandler to access class members
    MQTTHandler* handler = static_cast<MQTTHandler*>(obj);

    // Static members are used to access the non-static members
    JSONParser& jsonParser = handler->jsonParser;
    SQLiteDatabase& db = handler->db;

    // Handle the on_disconnect event
    if (rc == 0) {
        // Clean disconnect
        std::cout << "Disconnected from MQTT broker." << std::endl;
        // Add your logic here for a clean disconnect
    } else {
        // Unexpected disconnect or error
        std::cerr << "Unexpected disconnection from MQTT broker. Return code: " << rc << std::endl;
        // Add your error handling or reconnect logic here
    }
}

void MQTTHandler::on_subscribe(struct mosquitto* mosq, void* obj, int mid, int qos_count, const int* granted_qos) {
    // Cast obj back to MQTTHandler to access class members
    MQTTHandler* handler = static_cast<MQTTHandler*>(obj);

    // Handle the on_subscribe event
    std::cout << "Subscribed with message ID: " << mid << std::endl;

    // Add your logic for handling subscriptions here
}

void MQTTHandler::on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message) {
    // Cast obj back to MQTTHandler to access class members
    MQTTHandler* handler = static_cast<MQTTHandler*>(obj);

    // Static members are used to access the non-static members
    JSONParser& jsonParser = handler->jsonParser;
    SQLiteDatabase& db = handler->db;

    // Handle the on_message event
    std::cout << "Received message on topic: " << message->topic << std::endl;
    std::cout << "Message payload: " << static_cast<const char*>(message->payload) << std::endl;

    // Assuming you want to process the message here
    std::string payload(static_cast<const char*>(message->payload), message->payloadlen);

    // Add your decryption and data processing logic here
    std::map<std::string, std::string> decryptedData = jsonParser.parseAndDecrypt(payload, "YourEncryptionKeyHere");

    // Process decrypted data (store in database, etc.)
    std::string email = decryptedData["email"];
    std::string password = decryptedData["password"];

    // Generate a secure, random salt for each password
    SaltGenerator saltGenerator;
    std::string salt = saltGenerator.generateSalt(12);

    // Salt & Hash the password
    HashGenerator hashGenerator;
    std::string saltedPassword = password + salt;
    std::string hashedPassword = hashGenerator.computeSHA256(saltedPassword);

    // Insert user data into the users table or perform other actions
    if (!db.insert("users", "email, password, salt", "'" + email + "', '" + hashedPassword + "', '" + salt + "'")) {
        std::cerr << "Data insertion failed!" << std::endl;
        // Handle the error appropriately
    }
}