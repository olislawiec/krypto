#ifndef BEZPIECZESTWO_LIBCRYPT_H
#define BEZPIECZESTWO_LIBCRYPT_H

#include <string>
#include <openssl/ossl_typ.h>

std::string *generate_iv(unsigned int bits);

std::string *encrypt(std::string* plaintext_string, std::string *key,
                     std::string *iv);

std::string *encrypt(std::string* plaintext_string, std::string *key,
                     std::string *iv, const EVP_CIPHER *cipher);

std::string *decrypt(std::string* ciphertext_string, std::string *key,
                     std::string *iv);

std::string *decrypt(std::string* ciphertext_string, std::string *key,
                     std::string *iv, const EVP_CIPHER *cipher);

#endif
