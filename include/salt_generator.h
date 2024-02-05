#ifndef SALT_GENERATOR_H
#define SALT_GENERATOR_H

#include <string>

class SaltGenerator {
public:
    // Constructor and Destructor
    SaltGenerator();
    ~SaltGenerator();

    // Public Methods for generating a salt
    std::string generateSalt(size_t length);
};

#endif // SALT_GENERATOR_H