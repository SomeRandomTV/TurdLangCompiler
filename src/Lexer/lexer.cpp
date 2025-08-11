#include "lexer.hpp"
#include <iostream>

Lexer::Lexer(const std::string &source) : source(source) {}


std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    std::cout << "Tokenizing..." << std::endl;

    // get the first ch
    char ch = source[0];

    while (ch != '\0') {
        ch = source[current++];
        std::cout << ch << std::endl;
    }




    return tokens;
}


void Lexer::printTokens() const {
    for (const Token &token: tokens) {
        std::cout << "Token: " << token.lexeme
                << " | Type: " << static_cast<int>(token.type)
                << " | Line: " << token.line
                << " | Column: " << token.column << std::endl;
    }
}

void Lexer::printSource() const {
    std::cout << source << std::endl;
}

