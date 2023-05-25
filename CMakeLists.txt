cmake_minimum_required(VERSION 3.10)

project(vTCM)

include_directories(./include)

add_compile_options(-std=c++17 -Wall)

aux_source_directory(./src SRC)

add_executable(vTCM ${SRC})

set(CMAKE_BUILD_TYPE Debug)



