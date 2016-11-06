#include <libcrypt.hpp>
#include <base64.hpp>
#include <fstream>
#include <iostream>
#include <utils.hpp>
#include <keystore.hpp>
#include <openssl/evp.h>

const EVP_CIPHER* stringToEVP(std::string string);

void printHelp() {
    std::cout
    << "Usage:" << std::endl
    << "zadanie1 <encrypt/decrypt> <keystore> <id klucza> <sposob szyfrowania> <plik> <plik wynikowy>" << std::endl
    << "Sposoby szyfrowania: CBC (używany w odtwarzaczu), CTR, OFB" << std::endl;
}

int main(int argc, char **argv) {

    if (argc != 7) {
        printHelp();
        return -1;
    }

    if (std::string(argv[1]) == "encrypt") {

        auto cipher = stringToEVP(std::string(argv[4]));

        /* A 128 bit IV */
        auto *iv = generate_iv(128);

        printf("Proszę podać hasło\n");

        auto *password = getPasswordSecurely();

        std::string *contents = fileToString(argv[5]);

        auto keystore = keystore::loadFromFile(argv[2]);

        auto keyID = new std::string(argv[3]);

        std::string *key = keystore->getKey(*keyID, *password);

        std::string *encrypted = encrypt(contents, key, iv, cipher);

        std::ofstream myfile(argv[6], std::ios::trunc | std::ios::binary);
        myfile << base64_encode(*iv);
        myfile << "-";
        myfile << base64_encode(*encrypted);
        myfile.close();


    } else if (std::string(argv[1]) == "decrypt") {

        auto cipher = stringToEVP(std::string(argv[4]));

        printf("Proszę podać hasło\n");

        std::string *password = getPasswordSecurely();

        std::string *contents = fileToString(argv[5]);

        std::string iv_encode = contents->substr(0, contents->find("-"));
        std::string encrypted_encode = contents->substr(iv_encode.size() + 1, contents->size());

        std::string iv_decode = base64_decode(iv_encode);
        std::string encrypted_decode = base64_decode(encrypted_encode);

        auto keystore = keystore::loadFromFile(argv[2]);

        auto keyID = new std::string(argv[3]);

        std::string *key = keystore->getKey(*keyID, *password);

        std::string *decrypted = decrypt(&encrypted_decode, key, &iv_decode, cipher);

        std::ofstream myfile(argv[6], std::ios::trunc | std::ios::binary);
        myfile << *decrypted;
        myfile.close();

    }

    return 0;
}

const EVP_CIPHER* stringToEVP(std::string string) {

    if (string == "CBC") {
        return EVP_aes_256_cbc();
    } else if (string == "CTR") {
        return EVP_aes_256_ctr();
    } else if (string == "OFB") {
        return EVP_aes_256_ofb();
    }

    printf("Brak podanego sposobu szyfrowania\n");

    return nullptr;

}
