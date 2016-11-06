#include <keystore.hpp>
#include <libcrypt.hpp>
#include <base64.hpp>
#include <fstream>
#include <utils.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

void keystore::addGeneratedKey(std::string name, std::string password) {
    addKey(name, *generate_iv(256), password);
}

void keystore::addKey(std::string name, std::string key, std::string password) {

    std::string *random_iv = generate_iv(128);

    std::string *zaszyfrowany_klucz = encrypt(&key, &password, random_iv);

    keysMap[name] = new std::pair<std::string *, std::string *>(zaszyfrowany_klucz, random_iv);

}

std::string *keystore::getKey(std::string name, std::string password) {

    std::pair<std::string *, std::string *> *myPair = keysMap[name];

    if (myPair == nullptr) {
        return nullptr;
    }

    return decrypt(myPair->first, &password, myPair->second);
}


//<NAZWA>-<IV>-<KLUCZ>
void keystore::saveToFile(char *file) {

    std::string *data = new std::string;

    typedef std::map<std::string, std::pair<std::string *, std::string *> *>::iterator it_type;
    for (it_type iterator = keysMap.begin(); iterator != keysMap.end(); iterator++) {

        data->append(base64_encode(iterator->first));
        data->append("-");
        data->append(base64_encode(*iterator->second->second));
        data->append("-");
        data->append(base64_encode(*iterator->second->first));
        data->append("-");

    }

    //Remove last character
    if (!data->empty()) {
        data->erase(data->size() - 1, 1);;
    }

    std::ofstream fileStream;
    fileStream.open(file, std::ios::trunc | std::ios::binary);
    fileStream << *data;
    fileStream.close();

}

keystore *keystore::loadFromFile(char *file) {
    return keystore::loadFromString(fileToString(file));
}

keystore *keystore::loadFromString(std::string *string) {

    if (string->empty()) {
        return new keystore();
    }

    std::vector<std::string> splitVec;
    boost::split(splitVec, *string, boost::is_any_of("-"),
                 boost::token_compress_on);


    size_t i = 0;

    keyStoreMap map;

    while (i < splitVec.size()) {
        map[base64_decode(splitVec[i])] = new std::pair<std::string *, std::string *>(
                new std::string(base64_decode(splitVec[i + 2])),
                new std::string(base64_decode(splitVec[i + 1])));
        i = i + 3;
    }

    return new keystore(map);
}

std::pair<std::string *, std::string *> *keystore::directGetPair(std::string name) {
    return keysMap[name];
}

std::vector<std::string *> *keystore::getNames() {

    std::vector<std::string *> *vector = new std::vector<std::string *>;

    for (keyStoreMap::iterator it = keysMap.begin(); it != keysMap.end(); ++it) {
        vector->push_back(new std::string(it->first));
    }

    return vector;
}

void keystore::removeKey(std::string name) {
    // std::cout << "removing: " << name << std::endl;
    auto it = keysMap.find(name);
    keysMap.erase(it);
}

bool keystore::checkKeyExistence(std::string name) {
    return keysMap[name] != nullptr;
}
