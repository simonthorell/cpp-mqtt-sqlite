#include <iostream>
#include "mqtt_handler.h"
#include "aes256_decryptor.h"
#include "salt_generator.h"
#include "hash_generator.h"
#include "sqlite_database.h"

// TODO: Replace with secrets from a secure location
#define MQTT_BROKER_URI "tcp://localhost:1883"
#define MQTT_CLIENT_ID "client1"
#define MQTT_TOPIC "test/topic"
#define MQTT_QOS 1

int main() {
    // Create a new database
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

    // Initialize the MQTTHandler
    MQTTHandler mqtt(MQTT_BROKER_URI, MQTT_CLIENT_ID, MQTT_TOPIC, MQTT_QOS);

    mqtt.connect();    // Connect to the MQTT broker
    mqtt.subscribe();  // Subscribe to the topic

    // Wait for messages to arrive and handle here...
    while (true) {
        // TODO: Parse the JSON message from the MQTT topic
        // TODO: Decrypt email & password using AES256 Decryptor

        // Dummy user data (Replace with decrypted data from MQTT message)
        std::string email = "user@example.com";
        std::string password = "password"; // Store a hashed password, not plain text

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
            return -1;
        }
    }

    // Print all data from the users table in SQL database
    if (!db.selectAllFromTable("users")) {
        std::cerr << "Failed to select data!" << std::endl;
        return -1;
    }

    // Disconnect from the MQTT broker
    mqtt.disconnect();

    // Close the SQL database
    db.close();

    return EXIT_SUCCESS;
}