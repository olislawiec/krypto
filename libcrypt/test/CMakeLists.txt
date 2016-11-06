cmake_minimum_required(VERSION 2.8.12)
project(libcrypt_test)

enable_testing()
find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

add_executable(libcrypttests tests.cpp)
target_link_libraries(libcrypttests LINK_PUBLIC ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} LibCrypt)
set_target_properties(libcrypttests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
add_test(AllTestsInFoo libcrypttests)
