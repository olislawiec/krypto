cmake_minimum_required(VERSION 2.8.12)

project(libcrypt_lib)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_library(LibCrypt libcrypt.cpp ${OPENSSL_LIBRARIES} base64.cpp keystore.cpp utils.cpp)
target_link_libraries(LibCrypt ${OPENSSL_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

target_include_directories(LibCrypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
