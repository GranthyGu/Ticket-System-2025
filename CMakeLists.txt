cmake_minimum_required(VERSION 3.16)
project(MapListTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(code
        src/token_scanner.cpp
        src/main.cpp
)