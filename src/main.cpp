#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>

#include "mqtt_handler.h"
#include "json_parser.h"
#include "aes256_decryptor.h"
#include "salt_generator.h"
#include "hash_generator.h"
#include "sqlite_database.h"

// TODO: Replace with secrets from a secure location
#define MQTT_BROKER_URI "tcp://localhost:1883"
#define MQTT_CLIENT_ID "client1"
#define MQTT_TOPIC "test/topic"
#define MQTT_QOS 1

std::atomic<bool> interrupted(false);

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    interrupted.store(true);
}

int main() {
    // Set up signal handling to terminate the application
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create a new SQLite3 database
    SQLiteDatabase db("sql_database.db");

    // Define the schema for the users table
    std::string usersTableFields = "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "email TEXT NOT NULL, "
                                   "password TEXT NOT NULL, "
                                   "salt TEXT NOT NULL";

    // Create the users table
    if (!db.createTable("users", usersTableFields)) {
        std::cerr << "Table creation failed!" << std::endl;
        return -1;
    }

    // Create a new JSONParser (JSON parsing, base64/hex decoding and AES-256 decryption)
    JSONParser jsonParser;

    // Initialize the MQTTHandler
    MQTTHandler mqtt(MQTT_BROKER_URI, MQTT_CLIENT_ID, MQTT_TOPIC, MQTT_QOS, db, jsonParser);

    mqtt.connect();    // Connect to the MQTT broker
    mqtt.subscribe();  // Subscribe to the topic

    // Keep the program running until interrupted from signal
    while (!interrupted.load()) {
        // All incoming messages are handled by the MQTTHandler::handleMessage method
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Print all data from the users table in SQL database
    // if (!db.selectAllFromTable("users")) {
    //     std::cerr << "Failed to select data!" << std::endl;
    //     return -1;
    // }

    // Disconnect from the MQTT broker
    mqtt.disconnect();

    // Close the SQL database
    db.close();

    return EXIT_SUCCESS;
}