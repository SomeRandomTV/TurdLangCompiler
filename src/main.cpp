#include <iostream>
#include <ostream>
#include "Lexer/lexer.hpp"

int main(const int argc, char **argv) {

    if (argc != 2) {
        exit(1);
    }
    if (argv == nullptr) {
        exit(2);
    }

    const std::string source = argv[1];
    std::cout << source << std::endl;
    Lexer Lexer(source);
    Lexer.tokenize();
    std::cout << std::endl;

}