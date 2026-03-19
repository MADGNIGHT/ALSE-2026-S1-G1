cmake_minimum_required(VERSION 3.10)

project(Corte2_Ejer2)

set(CMAKE_CXX_STANDARD 17)

add_executable(design_spreadsheet design_spreadsheet.cpp)
add_executable(design_twitter design_twitter.cpp)
add_executable(queen_attack queen_attack.cpp)
add_executable(robot_simulator robot_simulator.cpp)

//Los programas se correr con ./nombre_programa