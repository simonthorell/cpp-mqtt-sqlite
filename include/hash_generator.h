#ifndef HASH_GENERATOR_H
#define HASH_GENERATOR_H

#include <string>

class HashGenerator {
public:
    // Constructor and Destructor
    HashGenerator();
    ~HashGenerator();

    // Public methods for computing hashes
    std::string computeMD5(const std::string& input);
    std::string computeSHA1(const std::string& input);
    std::string computeSHA256(const std::string& input);

private:
    // Private method to convert hash bytes to hex string
    std::string bytesToHexString(const unsigned char* data, unsigned int length);
};

#endif // HASH_GENERATOR_H