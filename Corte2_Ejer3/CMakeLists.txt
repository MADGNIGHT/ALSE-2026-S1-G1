cmake_minimum_required(VERSION 3.10)

project(MyPrograms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ejecutable principal (único con main)
add_executable(crypto_square
    crypto_square.cpp
    authentication.cpp
    bank.account.cpp
    exam_tracker.cpp
)