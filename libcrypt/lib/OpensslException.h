#ifndef PROJECT_OPENSSLEXCEPTION_H
#define PROJECT_OPENSSLEXCEPTION_H

#include <stdexcept>

class OpensslException : public std::runtime_error {
public:
    OpensslException() : std::runtime_error("OpenSSL exception") { }
};

#endif
