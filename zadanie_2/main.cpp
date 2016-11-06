#include <ao/ao.h>
#include <mpg123.h>
#include <iostream>
#include <utils.hpp>
#include <fstream>
#include <base64.hpp>
#include <libcrypt.hpp>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <keystore.hpp>
#include <OpensslException.h>
#include <thread>
#include <curses.h>
#include "song.hpp"
#include <boost/thread.hpp>

#define BITS 8

//Uber tajny klucz
std::string key = std::string("supertajnykluczdoodtwarzaczamp3_");
std::string plik = std::string(".mp3config");

void usage() {
    std::cout << "zadanie2 init <keystore> <id klucza>" << std::endl;
}

void usageWithConfig() {
    std::cout << "zadanie2 <zaszyfrowany plik>" << std::endl;
}

//Kanged from https://hzqtc.github.io/2012/05/play-mp3-with-libmpg123-and-libao.html
int main(int argc, char *argv[]) {


    if (argc > 1 && std::string(argv[1]) == "init") {

        auto sciezka = std::string(argv[2]);
        auto id_klucza = std::string(argv[3]);

        //Sprawdzamy, czy keystore, klucz i hasło się zgadzają

        FILE *file = fopen(sciezka.data(), "rb");

        if (file == nullptr) {
            std::cout << "Ścieżka do keystora jest niepoprawna" << std::endl;
            return -1;
        }

        keystore *keystore1 = keystore::loadFromFile((char *) sciezka.data());

        if (!keystore1->checkKeyExistence(id_klucza)) {
            std::cout << "Nazwa klucza jest niepoprawna" << std::endl;
            return -1;
        }


        std::cout << "Podaj hasło do klucza" << std::endl;
        auto haslo = getPasswordSecurely();


        try {
            keystore1->getKey(id_klucza, *haslo);
        } catch (OpensslException &e) {
            std::cout << "Błędne hasło" << std::endl;
            return -1;
        }


        std::cout << "Podaj PIN" << std::endl;
        auto pin = getPasswordSecurely();

        auto toWrite = std::string();


        toWrite.append(base64_encode(sciezka));
        toWrite.append("-");
        toWrite.append(base64_encode(id_klucza));
        toWrite.append("-");
        toWrite.append(base64_encode(*haslo));
        toWrite.append("-");
        toWrite.append(base64_encode(*pin));


        std::ofstream fileStream;
        fileStream.open(plik, std::ios::trunc | std::ios::binary);
        fileStream << *encrypt(&toWrite, &key, &key);
        fileStream.close();


        return 0;
    }


    FILE *file = fopen(plik.data(), "rb");

    if (file == nullptr) {
        usage();
        return -1;
    }

    fclose(file);

    if (argc == 1) {
        usageWithConfig();
        return -1;
    }

    auto configFile = fileToString((char *) plik.data());

    if (configFile->empty()) {
        std::cout << "No config file" << std::endl;
        return -1;
    }

    auto configFileDecrypted = decrypt(configFile, &key, &key);

    std::vector<std::string> splitVec;
    boost::split(splitVec, *configFileDecrypted, boost::is_any_of("-"),
                 boost::token_compress_on);

    auto sciezka = base64_decode(splitVec.data()[0]);
    auto id_klucza = base64_decode(splitVec.data()[1]);
    auto haslo = base64_decode(splitVec.data()[2]);
    std::string pin = base64_decode(splitVec.data()[3]);

    std::cout << "Podaj PIN" << std::endl;

    std::string *pinGet = getPasswordSecurely();

    if (pin != *pinGet) {
        std::cout << "Nieprawidłowy PIN" << std::endl;
        return -1;
    }


    auto keystore = keystore::loadFromFile((char *) sciezka.data());

    auto klucz = keystore->getKey(id_klucza, haslo);

    std::string *encryptedMusic = fileToString(argv[1]);

    std::string iv_encode = encryptedMusic->substr(0, encryptedMusic->find("-"));
    std::string encrypted_encode = encryptedMusic->erase(0, iv_encode.size() + 1);

    std::string iv_decode = base64_decode(iv_encode);
    std::string encrypted_decode = base64_decode(encrypted_encode);

    std::string *decryptedMusic = decrypt(&encrypted_decode, klucz, &iv_decode);

    song *song1 = new song(decryptedMusic);

    boost::thread t1(boost::bind(&song::play, song1));

    initscr();
    timeout(-1);

    int c = 0;

#define Z_KEY 122

#define A_KEY 97
#define D_KEY 100


    while (c != Z_KEY) {
        c = getch();

        if (c == A_KEY) {
            song1->prev();
        } else if (c == D_KEY) {
            song1->forward();
        }

    }


    endwin();

    song1->stop();
    t1.join();

    delete song1;

    return 0;
}
