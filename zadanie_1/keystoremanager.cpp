#include <string>
#include <keystore.hpp>
#include <iostream>
#include <utils.hpp>
#include <OpensslException.h>


void printHelp() {
    std::cout
    << "Usage:" << std::endl
    << "keystore list <plik>" << std::endl
    << "keystore add <nazwa> <plik>" << std::endl
    << "keystore remove <nazwa> <plik>" << std::endl
    << "keystore check <nazwa> <plik>" << std::endl;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        printHelp();
        return -1;
    }

    if (std::string(argv[1]) == "list") {

        keystore *myKeystore = keystore::loadFromFile(argv[2]);

        auto vector = myKeystore->getNames();

        for (const auto* i : *vector) {
            std::cout << *i << std::endl;
        }

    } else if (std::string(argv[1]) == "add") {

        keystore *myKeystore = keystore::loadFromFile(argv[3]);

        std::cout << "Wprowadź hasło" << std::endl;

        std::string* key = getPasswordSecurely();

        std::cout << "Wprowadź hasło ponownie" << std::endl;

        std::string* key2 = getPasswordSecurely();

        if (*key != *key2) {
            std::cout << "Hasła nie są itentyczne" << std::endl;
        }

        myKeystore->addGeneratedKey(*new std::string(argv[2]), *key);

        myKeystore->saveToFile(argv[3]);

    } else if (std::string(argv[1]) == "remove") {

        keystore *myKeystore = keystore::loadFromFile(argv[3]);

        myKeystore->removeKey(std::string(argv[2]));
        myKeystore->saveToFile(argv[3]);

    } else if (std::string(argv[1]) == "check") {

        std::cout << "Wprowadź hasło" << std::endl;

        std::string* key = getPasswordSecurely();

        keystore *myKeystore = keystore::loadFromFile(argv[3]);

        try {
            myKeystore->getKey(std::string(argv[2]), *key);
        } catch (OpensslException &e) {
            std::cout << "Błędne hasło" << std::endl;
            return -1;
        }

        std::cout << "Prawidłowe hasło" << std::endl;

    } else {
        printHelp();
        return -1;
    }


}
