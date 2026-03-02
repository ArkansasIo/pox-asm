cmake_minimum_required(VERSION 3.16)
project(pox_dual C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(common)
add_subdirectory(pox8)
add_subdirectory(pox64)
add_subdirectory(tools)
