#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>

#include "config.h"
#include "mqtt_handler.h"
#include "json_parser.h"
#include "aes256_decryptor.h"
#include "salt_generator.h"
#include "hash_generator.h"
#include "sqlite_database.h"

std::atomic<bool> interrupted(false);

// Signal handler to terminate the application
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    interrupted.store(true);
}

int main() {
    // Load the configuration file (default if config.json does not exist)
    Config config("config_default.json", "config.json");

    // Set up signal handling to terminate the application
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create a new SQLite3 database
    SQLiteDatabase db("sql_database.db");

    // Define the schema for the users tablec
    std::string usersTableFields = "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "email TEXT NOT NULL, "
                                   "password TEXT NOT NULL, "
                                   "salt TEXT NOT NULL";

    // Create the users table
    if (!db.createTable("users", usersTableFields)) {
        std::cerr << "Table creation failed!" << std::endl;
        return -1;
    }

    // Create new JSONParser (Also base64/hex decode and AES-256 decrypt)
    JSONParser jsonParser;

    // Create an instance of MQTTHandler and pass the required parameters
    MQTTHandler mqtt(
        config.getMqttClientId(),       // MQTT Client ID
        config.getMqttBrokerURI(),      // MQTT Broker URI
        config.getMqttBrokerPort(),     // MQTT Broker Port
        config.getMqttCaCertificate(),   // CA Certificate
        config.getMqttUsername(),       // MQTT Username
        config.getMqttPassword(),       // MQTT Password
        jsonParser,                    // JSONParser instance
        db                             // SQLiteDatabase instance
    );

    // Connect to the MQTT broker
    mqtt.connect();
    // Subscribe to the topic (casting string to const char*)
    mqtt.subscribe(config.getMqttTopic()); 

    // Keep the program running until interrupted from signal
    while (!interrupted.load()) {
        // Incoming messages are handled by MQTTHandler::handleMessage
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