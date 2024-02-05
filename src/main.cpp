#include <iostream>
#include <openssl/evp.h> // OpenSSL Crypto library
#include <sqlite3.h>     // SQLite3 library
#include <mqtt/client.h> // MQTT library
#include "test_file.h"

int main() {

    int testing = test_method();
    std::cout << "Testing: " << testing << std::endl;
    return 0;
}