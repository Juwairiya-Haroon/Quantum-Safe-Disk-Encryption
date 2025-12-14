cmake_minimum_required(VERSION 3.10)
project(Member1Crypto)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_crypto
    src/crypto/AES.cpp
    src/crypto/PQC.cpp
    src/crypto/HybridEncryption.cpp
    src/crypto/test_crypto.cpp
)

target_include_directories(test_crypto PUBLIC src/crypto)
