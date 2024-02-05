#include "json_parser.h"
#include <openssl/evp.h>
#include <openssl/buffer.h> // base64 decoding
#include <openssl/bio.h> // base64 decoding
#include <iostream>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

std::map<std::string, std::string> JSONParser::parseAndDecrypt(const std::string& jsonData, const std::string& key) {
    // Parse JSON data
    auto json = nlohmann::json::parse(jsonData);
    
    // Decode and decrypt the email and password
    AES256Decryptor decryptor;
    std::string decryptedEmail = decryptor.decrypt(base64Decode(json["email"]), key, hexToBytes(json["iv"]));
    std::string decryptedPassword = decryptor.decrypt(base64Decode(json["password"]), key, hexToBytes(json["iv"]));
    
    // Store and return the decrypted email and password
    std::map<std::string, std::string> result;
    result["email"] = decryptedEmail;
    result["password"] = decryptedPassword;
    return result;
}

std::string JSONParser::base64Decode(const std::string& encoded) {
    BIO *bio, *b64;
    int decodeLen = calcDecodeLength(encoded.c_str(), encoded.length());
    auto* buffer = (unsigned char*)malloc(decodeLen + 1);
    buffer[decodeLen] = '\0';

    bio = BIO_new_mem_buf(encoded.c_str(), -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Do not use newlines to flush buffer
    int length = BIO_read(bio, buffer, encoded.length());
    BIO_free_all(bio);

    std::string decodedString((char*)buffer, length);
    free(buffer);
    return decodedString;
}

int JSONParser::calcDecodeLength(const char* b64input, int length) {
    int padding = 0;

    // Check for trailing '=''s as padding
    if (b64input[length-1] == '=' && b64input[length-2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[length-1] == '=') //last char is =
        padding = 1;

    return (int)length * 0.75 - padding;
}

std::string JSONParser::hexToBytes(const std::string& hex) {
    // Convert hexadecimal string to bytes
    std::string bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes += byte;
    }
    return bytes;
}