cmake_minimum_required(VERSION 2.8.12)
project(zadanie_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

add_executable(zadanie1 main.cpp)
target_link_libraries(zadanie1 LINK_PUBLIC LibCrypt)
set_target_properties(zadanie1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(keystore keystoremanager.cpp)
target_link_libraries(keystore LINK_PUBLIC LibCrypt)
set_target_properties(keystore PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
