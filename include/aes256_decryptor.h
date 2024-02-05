#ifndef AES256_DECRYPTOR_H
#define AES256_DECRYPTOR_H

#include <string>
#include <openssl/evp.h>

class AES256Decryptor {
public:
    AES256Decryptor();
    ~AES256Decryptor();

    std::string decrypt(const std::string& ciphertext, const std::string& key, const std::string& iv);
};

#endif // AES256_DECRYPTOR_H