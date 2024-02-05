//=============================================================================
// SaltGenerator Class Implementation
//=============================================================================

#include "salt_generator.h"
#include <random>

//=============================================================================
// Constructor and Destructor: SaltGenerator, ~SaltGenerator
// Description: Constructor and Destructor for the SaltGenerator class.
//=============================================================================

SaltGenerator::SaltGenerator() = default;

SaltGenerator::~SaltGenerator() = default;

//=============================================================================
// Public Method: generateSalt
// Description: Generates a random salt of the specified length.
//=============================================================================

std::string SaltGenerator::generateSalt(size_t length) {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);

    std::string salt;
    salt.reserve(length);

    // Create a random number generator
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, max_index - 1);

    for (size_t i = 0; i < length; ++i) {
        salt += charset[distribution(generator)];
    }

    return salt;
}