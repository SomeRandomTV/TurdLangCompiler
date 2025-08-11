#pragma once
#include <string>
#include <vector>

#pragma once

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
    STR_LIT             // "text" or 'text'
};


typedef struct Token {
    std::string lexeme;     // code lexeme
    TokenType type;         // corresponding token
    int line;               // line number
    int column;             // column number
} Token;

class Lexer {
    public:
    explicit Lexer(const std::string &source);     // initialize the lexer and open the file
    void printTokens() const;
    void printSource() const;
    std::vector<Token> tokenize();


private:
    std::string source;           // entire source code from file
    std::vector<Token> tokens;    // all tokens found
    size_t current = 0;           // current character index
    int line = 1;                 // current line number
    int column = 1;               // current column number

    // Helpers
    char peek() const;
    char peekNext() const;

    // Tokenizing rules
    void identifier();          // tokenizing for idents
    void number();              // tokeninzing for int_lit and float_lit
    void stringLiteral();       // string tokenizing
};
