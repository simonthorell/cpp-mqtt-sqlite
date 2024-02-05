#ifndef SQLITE_DATABASE_H
#define SQLITE_DATABASE_H

#include <sqlite3.h>
#include <string>

class SQLiteDatabase {
private:
    sqlite3* db;
    char* errMsg;
    int rc;
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);

public:
    SQLiteDatabase(const std::string& databaseName);
    ~SQLiteDatabase();
    
    bool execute(const std::string& sql);
    bool createTable(const std::string& tableName, const std::string& fields);
    bool insert(const std::string& tableName, const std::string& fields, const std::string& values);
    void close();
    bool selectAllFromTable(const std::string& tableName);
};

#endif // SQLITE_DATABASE_H