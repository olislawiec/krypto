cmake_minimum_required(VERSION 2.8.12)
project(zadanie_2)

find_package(LibAO REQUIRED)
find_package(MPG123 REQUIRED)
find_package(Threads REQUIRED)
find_package(Curses REQUIRED)
find_package(Boost COMPONENTS system thread REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

add_executable(zadanie2 main.cpp song.cpp)
target_link_libraries(zadanie2 LINK_PUBLIC LibCrypt ${LIBAO_LIBRARIES} ${MPG123_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${CURSES_LIBRARIES})
set_target_properties(zadanie2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
