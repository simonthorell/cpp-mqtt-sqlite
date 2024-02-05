#include <iostream>
#include "mqtt_handler.h"
#include "aes_decryptor.h"
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

    // Define user data
    std::string email = "user@example.com";
    std::string password = "password123"; // Store a hashed password, not plain text
    std::string salt = "random_salt"; // Generate a secure, random salt for each password

    // Insert user data into the users table
    if (!db.insert("users", "email, password, salt", "'" + email + "', '" + password + "', '" + salt + "'")) {
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