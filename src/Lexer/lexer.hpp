#pragma once
#include <string>
#include <vector>
#include <fstream>

enum class TokenType {
    // Keywords
    KEY_PRINT,          // print
    KEY_IF,             // if
    KEY_ELSE,           // else
    KEY_READ,           // read
    KEY_WHILE,          // while
    KEY_FOR,            // for

    // Delimiters
    LEFT_PAREN,         // (
    RIGHT_PAREN,        // )
    LEFT_BRACE,         // {
    RIGHT_BRACE,        // }
    LEFT_BRACKET,       // [
    RIGHT_BRACKET,      // ]
    COMMA,              // ,
    SEMICOLON,          // ;
    COLON,              // :

    // Operators
    ADD_OP,             // +
    SUB_OP,             // -
    MUL_OP,             // *
    DIV_OP,             // /
    MOD_OP,             // %
    POW_OP,             // **
    AND_OP,             // &&
    OR_OP,              // ||
    NOT_OP,             // !
    INT_DIV_OP,         // //
    ASSIGN_OP,          // =
    EQUAL_OP,           // ==
    NOT_EQUAL_OP,       // !=
    GREATER_OP,         // >
    GEQUAL_OP,          // >=
    LESSER_OP,          // <
    LEQUAL_OP,          // <=

    // Data types
    DATATYPE_INT,       // int
    DATATYPE_FLOAT,     // float
    DATATYPE_STRING,    // string

    // Literals / identifiers
    IDENTIFIER,         // variable name
    INT_LIT,            // 123
    FLOAT_LIT,          // 123.456
    STR_LIT,             // "text" or 'text'

    UNKNOWN,            // unknown token


};


/*
 * Token Struct
 *
 * Holds the lexeme and token type
 */
struct Token {
    std::string lexeme;      // lexeme built
    TokenType type;   // type of token
    int line;           // line number
    int column;         // column number

    // struct constructor
    Token() {
        lexeme = "";
        type = TokenType::UNKNOWN;
        line = 1;
        column = 1;
    }
};

/*
 * Lexer class
 *
 * Holds all the functions for lexical analysis
 */
class Lexer {
public:

    // constructor that takes in file name and checks if opened
    explicit Lexer(const std::string& source);
    void tokenize();      // parses source code collecting and classifying them
    TokenType get_token(const std::string &lexeme);
    std::vector<Token> tokens;   // holds all tokens


private:

    std::ifstream file;          // input file stream
    Token token;            // holds the lexeme and token type
    int line;                // line number
    int column;              // column number

};


