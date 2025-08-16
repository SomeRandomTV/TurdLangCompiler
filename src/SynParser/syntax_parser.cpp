#include "syntax_parser.hpp"
#include <iostream>
#include <stdexcept>

// Helper function to convert TokenType to string
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INT_LIT: return "INT_LIT";
        case TokenType::FLOAT_LIT: return "FLOAT_LIT";
        case TokenType::STR_LIT: return "STR_LIT";
        case TokenType::KEY_IF: return "KEY_IF";
        case TokenType::KEY_ELSE: return "KEY_ELSE";
        case TokenType::KEY_WHILE: return "KEY_WHILE";
        case TokenType::KEY_FOR: return "KEY_FOR";
        case TokenType::KEY_PRINT: return "KEY_PRINT";
        case TokenType::KEY_READ: return "KEY_READ";
        case TokenType::DATATYPE_INT: return "DATATYPE_INT";
        case TokenType::DATATYPE_FLOAT: return "DATATYPE_FLOAT";
        case TokenType::DATATYPE_STRING: return "DATATYPE_STRING";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::ASSIGN_OP: return "ASSIGN_OP";
        case TokenType::EQUAL_OP: return "EQUAL_OP";
        case TokenType::NOT_EQUAL_OP: return "NOT_EQUAL_OP";
        case TokenType::LESSER_OP: return "LESSER_OP";
        case TokenType::LEQUAL_OP: return "LEQUAL_OP";
        case TokenType::GREATER_OP: return "GREATER_OP";
        case TokenType::GEQUAL_OP: return "GEQUAL_OP";
        case TokenType::ADD_OP: return "ADD_OP";
        case TokenType::SUB_OP: return "SUB_OP";
        case TokenType::MUL_OP: return "MUL_OP";
        case TokenType::DIV_OP: return "DIV_OP";
        case TokenType::MOD_OP: return "MOD_OP";
        case TokenType::POW_OP: return "POW_OP";
        case TokenType::AND_OP: return "AND_OP";
        case TokenType::OR_OP: return "OR_OP";
        case TokenType::NOT_OP: return "NOT_OP";
        case TokenType::INT_DIV_OP: return "INT_DIV_OP";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

// === Constructor ===
SyntaxParser::SyntaxParser(const std::vector<Token> &tokens)
    : tokens(tokens), current(0) {}

// === Utility Functions ===
const Token& SyntaxParser::peek() const {
    if (current >= tokens.size()) {
        static Token eofToken{"", TokenType::END_OF_FILE, -1, -1};
        return eofToken;
    }
    return tokens[current];
}

const Token& SyntaxParser::advance() {
    if (!isAtEnd()) {
        current++;
    }
    return tokens[current - 1];
}

bool SyntaxParser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool SyntaxParser::isAtEnd() const {
    return current >= tokens.size() || peek().type == TokenType::END_OF_FILE;
}

void SyntaxParser::match(TokenType type) {
    if (peek().type == type) {
        advance();
    } else {
        error("Expected " + tokenTypeToString(type) + " but found " +
            tokenTypeToString(peek().type), peek().line, peek().column);
    }
}

bool SyntaxParser::consume(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void SyntaxParser::setSourceLocation(ASTNodePTR node, const Token& token) const {
    node->line = token.line;
    node->column = token.column;
}

[[noreturn]] void SyntaxParser::error(const std::string& msg, int line, int column) {
    throw std::runtime_error("Parse error at line " + std::to_string(line) +
                            ", column " + std::to_string(column) + ": " + msg);
}

ASTNodePTR SyntaxParser::parse_program() {
    auto programNode = std::make_shared<ProgramNode>();
    std::cout << "Parsing program..." << std::endl;
    std::cout << "Total tokens: " << tokens.size() << std::endl;
    std::cout << "Current token: " << peek().lexeme << " (" << tokenTypeToString(peek().type) << ")" << std::endl;
    return programNode;
}

