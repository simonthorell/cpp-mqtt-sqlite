#include "sqlite_database.h"
#include <iostream>

SQLiteDatabase::SQLiteDatabase(const std::string& databaseName) {
    rc = sqlite3_open(databaseName.c_str(), &db);
    if (rc) {
        std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }
}

SQLiteDatabase::~SQLiteDatabase() {
    sqlite3_close(db);
}

int SQLiteDatabase::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed; // Explicitly cast to void
    (void)azColName; // Explicitly cast to void
    
    for (int i = 0; i < argc; i++) {
        std::cout << (i > 0 ? ":" : "") // Add a colon before each field after the first
                  << (argv[i] ? argv[i] : "NULL");
    }
    std::cout << std::endl; // End the line after printing all fields of a row
    return 0;
}

bool SQLiteDatabase::execute(const std::string& sql) {
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool SQLiteDatabase::createTable(const std::string& tableName, const std::string& fields) {
    std::string sqlCreateTable = "CREATE TABLE IF NOT EXISTS " + tableName + "(" + fields + ");";
    return execute(sqlCreateTable);
}

bool SQLiteDatabase::insert(const std::string& tableName, const std::string& fields, const std::string& values) {
    std::string sqlInsert = "INSERT INTO " + tableName + " (" + fields + ") VALUES (" + values + ");";
    return execute(sqlInsert);
}

void SQLiteDatabase::close() {
    sqlite3_close(db);
}

bool SQLiteDatabase::selectAllFromTable(const std::string& tableName) {
    std::string sqlSelectAll = "SELECT * FROM " + tableName + ";";
    return execute(sqlSelectAll);
}