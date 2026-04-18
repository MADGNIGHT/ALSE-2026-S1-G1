cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(EjerciciosCorte3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ejecutables
add_executable(atm atm_system.cpp)
add_executable(auction auction_system.cpp)
add_executable(meetup meetup.cpp)
add_executable(underground underground_system.cpp)