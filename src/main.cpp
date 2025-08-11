#include "Lexer/lexer.hpp"

int main() {

    std::string source = "int main() { return 0; }";
    Lexer Lexer(source);
    Lexer.printSource();
    Lexer.tokenize();

}