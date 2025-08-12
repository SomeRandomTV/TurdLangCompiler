#include "lexer.hpp"
#include <iostream>
#include <fstream>
#include <valarray>

Lexer::Lexer(const std::string &source) {
    file.open(source);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + source);
    }
    line = 0;
    column = 0;
}

void Lexer::tokenize() {
    std::cout << "Tokenizing..." << std::endl;
    tokens.clear();
    char ch;

    while (file.get(ch)) {
        if (isspace(ch)) {
            if (ch == '\n') {
                line++;
                column = 0;
            }
            else {
                column++;
            }
            continue;
        }

        std::string current_lexeme;
        Token token;

        // Operators & punctuation
        if (strchr("(){}[]!|/;:<>=+-*", ch)) {
            current_lexeme += ch;

            char peek = file.peek();
            if (peek != EOF) {
                std::string two_char_op = current_lexeme + peek;
                if (get_token(two_char_op) != TokenType::UNKNOWN) {
                    current_lexeme = two_char_op;
                    file.get(); // consume peek
                    column++;
                }
            }

            token.lexeme = current_lexeme;
            token.type = get_token(current_lexeme);
            tokens.push_back(token);
            std::cout << "Token: \t" << token.lexeme
                      << " \t Type: \t" << static_cast<int>(token.type) << std::endl;
            column++;
            continue;
        }

        // Identifiers / keywords
        if (isalpha(ch) || ch == '_') {
            do {
                current_lexeme += ch;
                if (!file.get(ch)) break;
                column++;
            } while (isalnum(ch) || ch == '_');

            if (!isspace(ch) || !strchr("(){}[]!|/;:<>=+-*", ch)) {
                file.unget();
                column--;
            }

            token.lexeme = current_lexeme;
            token.type = get_token(current_lexeme);
            tokens.push_back(token);
            std::cout << "Token: \t" << token.lexeme
                      << " \t Type: \t" << static_cast<int>(token.type) << std::endl;
            continue;
        }

        // Numbers (support floats with one dot)
        if (isdigit(ch) || ch == '.') {
            bool dot_seen = (ch == '.');
            do {
                current_lexeme += ch;
                if (!file.get(ch)) break;
                column++;
                if (ch == '.') {
                    if (dot_seen) break; // second dot, stop token
                    dot_seen = true;
                    token.type = TokenType::FLOAT_LIT;
                }
                token.type = TokenType::INT_LIT;
            } while (isdigit(ch) || ch == '.');

            if (!isspace(ch) || !strchr("(){}[]!|/;:<>=+-*", ch)) {
                file.unget();
                column--;
            }

            token.lexeme = current_lexeme;
            tokens.push_back(token);
            std::cout << "Token: \t" << token.lexeme
                      << " \t Type: \t" << static_cast<int>(token.type) << std::endl;
            continue;
        }

        // Unknown tokens
        token.lexeme = std::string(1, ch);
        token.type = TokenType::UNKNOWN;
        tokens.push_back(token);
        column++;
    }
}

TokenType Lexer::get_token(const std::string &lexeme) {
    // Keywords
    if (lexeme == "print") return TokenType::KEY_PRINT;
    if (lexeme == "if") return TokenType::KEY_IF;
    if (lexeme == "else") return TokenType::KEY_ELSE;
    if (lexeme == "read") return TokenType::KEY_READ;
    if (lexeme == "while") return TokenType::KEY_WHILE;
    if (lexeme == "for") return TokenType::KEY_FOR;

    // Data types
    if (lexeme == "int") return TokenType::DATATYPE_INT;
    if (lexeme == "float") return TokenType::DATATYPE_FLOAT;
    if (lexeme == "string") return TokenType::DATATYPE_STRING;

    // Operators (multi-character)
    if (lexeme == "**") return TokenType::POW_OP;
    if (lexeme == "&&") return TokenType::AND_OP;
    if (lexeme == "||") return TokenType::OR_OP;
    if (lexeme == "==") return TokenType::EQUAL_OP;
    if (lexeme == "!=") return TokenType::NOT_EQUAL_OP;
    if (lexeme == ">=") return TokenType::GEQUAL_OP;
    if (lexeme == "<=") return TokenType::LEQUAL_OP;
    if (lexeme == "//") return TokenType::INT_DIV_OP;

    // Single-character operators and delimiters
    if (lexeme.length() == 1) {
        switch (lexeme[0]) {
            case '(': return TokenType::LEFT_PAREN;
            case ')': return TokenType::RIGHT_PAREN;
            case '{': return TokenType::LEFT_BRACE;
            case '}': return TokenType::RIGHT_BRACE;
            case '[': return TokenType::LEFT_BRACKET;
            case ']': return TokenType::RIGHT_BRACKET;
            case ',': return TokenType::COMMA;
            case ';': return TokenType::SEMICOLON;
            case ':': return TokenType::COLON;
            case '+': return TokenType::ADD_OP;
            case '-': return TokenType::SUB_OP;
            case '*': return TokenType::MUL_OP;
            case '/': return TokenType::DIV_OP;
            case '%': return TokenType::MOD_OP;
            case '!': return TokenType::NOT_OP;
            case '=': return TokenType::ASSIGN_OP;
            case '>': return TokenType::GREATER_OP;
            case '<': return TokenType::LESSER_OP;
                default: return TokenType::UNKNOWN;
        }
    }

    return TokenType::UNKNOWN;
}



