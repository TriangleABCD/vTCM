cmake_minimum_required(VERSION 3.10)

project(vTCM)

include_directories(./include)

add_compile_options(-std=c++17 -Wall -O3)

aux_source_directory(./src SRC)

add_executable(vTCM ${SRC})

find_package(OpenMP REQUIRED)
target_link_libraries(vTCM PRIVATE OpenMP::OpenMP_CXX)

target_link_libraries(vTCM PRIVATE usb-1.0)

target_link_libraries(vTCM PRIVATE lxc)

set(CMAKE_BUILD_TYPE Debug)



