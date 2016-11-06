#ifndef PROJECT_KEYSTORE_H
#define PROJECT_KEYSTORE_H

#include <string>
#include <map>
#include <vector>

//         nazwa                        zaszyfrowany klucz  iv
typedef std::map<std::string, std::pair<std::string *, std::string *> *> keyStoreMap;

class keystore {

private:
    keyStoreMap keysMap;

public:
    keystore() { }

    keystore(keyStoreMap map) : keysMap(map) { }

    //Klucz w postaci jawnej + hasło jakich chcemy go zaszyfrować
    void addKey(std::string name, std::string key, std::string password);

    std::string *getKey(std::string name, std::string password);

    //Direct access to map (mostly for tests)
    std::pair<std::string *, std::string *> *directGetPair(std::string name);

    void removeKey(std::string name);

    void saveToFile(char *file);

    std::vector<std::string*>* getNames();

    static keystore *loadFromFile(char *file);

    static keystore *loadFromString(std::string *string);

    void addGeneratedKey(std::string name, std::string password);

    bool checkKeyExistence(std::string name);

};


#endif
