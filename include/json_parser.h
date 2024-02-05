#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <nlohmann/json.hpp>
#include "aes256_decryptor.h"

class JSONParser {
public:
    std::map<std::string, std::string> parseAndDecrypt(const std::string& jsonData, const std::string& key);
private:
    std::string base64Decode(const std::string& encoded);
    int calcDecodeLength(const char* b64input, int length);
    std::string hexToBytes(const std::string& hex);
};

#endif // JSON_PARSER_H