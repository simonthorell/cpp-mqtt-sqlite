#include <iostream>
#include "mqtt_handler.h"
#include "aes256_decryptor.h"
#include "salt_generator.h"
#include "hash_generator.h"
#include "sqlite_database.h"

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

    // TODO: Subscribe to the MQTT "credentials" topic

    // TODO: Parse the JSON message from the MQTT topic

    // TODO: Decrypt the MQTT message using AES256 Decryptor

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

    // Select and print all entries from the "users" table
    if (!db.selectAllFromTable("users")) {
        std::cerr << "Failed to select data!" << std::endl;
        return -1;
    }

    // Close the database
    db.close();

    return EXIT_SUCCESS;
}