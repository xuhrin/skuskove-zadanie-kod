cmake_minimum_required(VERSION 3.20)

project("Pong")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(SDL2 REQUIRED)

add_subdirectory("ThirdParty")

add_subdirectory("Pong")
