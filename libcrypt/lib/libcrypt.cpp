#include "libcrypt.hpp"
#include <openssl/evp.h>
#include <fstream>
#include <openssl/rand.h>
#include "OpensslException.h"

void handleErrors(void) {
    throw OpensslException();
}

std::string *encrypt(std::string *plaintext_string, std::string *key_dangerous,
                     std::string *iv_dangerous, const EVP_CIPHER *cipher) {

    std::string key = *key_dangerous;
    std::string iv = *iv_dangerous;

    //256 bit key
    while (key.size() < cipher->key_len) {
        key.append("a");
    }

    //128 bit iv
    while (iv.size() < cipher->iv_len) {
        iv.append("a");
    }

    unsigned char *ciphertext = new unsigned char[plaintext_string->size() * 2];

    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if (1 != EVP_EncryptInit_ex(ctx, cipher, NULL, (const unsigned char *) key.data(),
                                (const unsigned char *) iv.data()))
        handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, (const unsigned char *) plaintext_string->c_str(),
                               plaintext_string->size()))
        handleErrors();
    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return new std::string((char *) ciphertext, ciphertext_len);

}

std::string *encrypt(std::string *plaintext_string, std::string *key_dangerous,
                     std::string *iv_dangerous) {
    return encrypt(plaintext_string, key_dangerous, iv_dangerous, EVP_aes_256_cbc());
}

std::string *decrypt(std::string *ciphertext_string, std::string *key_dangerous,
                     std::string *iv_dangerous, const EVP_CIPHER *cipher) {


    std::string key = *key_dangerous;
    std::string iv = *iv_dangerous;

    //256 bit key
    while (key.size() < cipher->key_len) {
        key.append("a");
    }

    //128 bit iv
    while (iv.size() < cipher->iv_len) {
        iv.append("a");
    }

    unsigned char *plaintext = new unsigned char[ciphertext_string->size()*2];

    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if (1 != EVP_DecryptInit_ex(ctx, cipher, NULL, (const unsigned char *) key.data(),
                                (const unsigned char *) iv.data()))
        handleErrors();

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, (const unsigned char *) ciphertext_string->c_str(),
                               ciphertext_string->size()))
        handleErrors();
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return new std::string((char *) plaintext, plaintext_len);

}

std::string *decrypt(std::string *ciphertext_string, std::string *key_dangerous,
                     std::string *iv_dangerous) {
    return decrypt(ciphertext_string, key_dangerous, iv_dangerous, EVP_aes_256_cbc());
}

std::string *generate_iv(unsigned int bits) {
    unsigned char *iv = new unsigned char[bits / 8];
    RAND_bytes(iv, bits / 8);
    return new std::string((char *) iv);
}
