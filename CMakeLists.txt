cmake_minimum_required(VERSION 3.31)
project(Compiler)

set(CMAKE_CXX_STANDARD 26)

add_executable(Compiler
        main.cpp
        lexer.cpp
)
