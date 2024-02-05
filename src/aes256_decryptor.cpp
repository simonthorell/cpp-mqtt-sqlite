#include "aes256_decryptor.h"
#include <openssl/evp.h> // OpenSSL Crypto library
#include <iostream>
#include <cstring>

AES256Decryptor::AES256Decryptor() {
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
}

AES256Decryptor::~AES256Decryptor() {
    // Clean up OpenSSL
    EVP_cleanup();
}

std::string AES256Decryptor::decrypt(const std::string& ciphertext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return "";
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.data()), reinterpret_cast<const unsigned char*>(iv.data())) != 1) {
        std::cerr << "Failed to initialize decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    std::string plaintext;
    plaintext.resize(ciphertext.size());
    int len = 0;
    int plaintext_len = 0;

    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &len, reinterpret_cast<const unsigned char*>(ciphertext.data()), ciphertext.size()) != 1) {
        std::cerr << "Failed to decrypt" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]) + len, &len) != 1) {
        std::cerr << "Failed to finalize decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}
