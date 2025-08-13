#include "lexer.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>

const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"print", TokenType::KEY_PRINT}, {"if", TokenType::KEY_IF},
    {"else", TokenType::KEY_ELSE}, {"read", TokenType::KEY_READ},
    {"while", TokenType::KEY_WHILE}, {"for", TokenType::KEY_FOR},
    {"int", TokenType::DATATYPE_INT}, {"float", TokenType::DATATYPE_FLOAT},
    {"string", TokenType::DATATYPE_STRING}
};

const std::unordered_map<std::string, TokenType> Lexer::operators_ = {
    {"**", TokenType::POW_OP}, {"&&", TokenType::AND_OP},
    {"||", TokenType::OR_OP}, {"==", TokenType::EQUAL_OP},
    {"!=", TokenType::NOT_EQUAL_OP}, {">=", TokenType::GEQUAL_OP},
    {"<=", TokenType::LEQUAL_OP}, {"//", TokenType::INT_DIV_OP}
};

const std::unordered_map<char, TokenType> Lexer::single_char_tokens_ = {
    {'(', TokenType::LEFT_PAREN}, {')', TokenType::RIGHT_PAREN},
    {'{', TokenType::LEFT_BRACE}, {'}', TokenType::RIGHT_BRACE},
    {'[', TokenType::LEFT_BRACKET}, {']', TokenType::RIGHT_BRACKET},
    {',', TokenType::COMMA}, {';', TokenType::SEMICOLON}, {':', TokenType::COLON},
    {'+', TokenType::ADD_OP}, {'-', TokenType::SUB_OP}, {'*', TokenType::MUL_OP},
    {'/', TokenType::DIV_OP}, {'%', TokenType::MOD_OP}, {'!', TokenType::NOT_OP},
    {'=', TokenType::ASSIGN_OP}, {'>', TokenType::GREATER_OP}, {'<', TokenType::LESSER_OP}
};

/**
 * Initialize the Lexer and open the source file
 * and reserve 1000 token space for now
 * @param filename file containing source code
 */
Lexer::Lexer(const std::string& filename) : file_(filename) {
    if (!file_.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    tokens_.reserve(1000);
}

[[noreturn]] void Lexer::lexer_error(const std::string& msg, int line, int column) {
    std::ostringstream oss;
    oss << "Error at line " << line << ", col " << column << ": " << msg;
    throw std::runtime_error(oss.str());
}

std::vector<Token> Lexer::tokenize() {
    tokens_.clear();

    while (!is_at_end()) {
        skip_whitespace();
        if (is_at_end()) break;

        char current = advance();
        int token_column = column_ - 1; // Store column where token starts

        try {
            if (current == '"' || current == '\'') {
                tokens_.push_back(read_string_literal(current));
            }
            else if (std::isalpha(current) || current == '_') {
                file_.unget();
                column_--;
                tokens_.push_back(read_identifier());
            }
            else if (std::isdigit(current) || current == '.') {
                file_.unget();
                column_--;
                tokens_.push_back(read_number());
            }
            else if (auto it = single_char_tokens_.find(current); it != single_char_tokens_.end()) {
                const char next = peek();
                std::string two_char = std::string(1, current) + next;

                if (auto op_it = operators_.find(two_char); op_it != operators_.end()) {
                    advance();
                    tokens_.push_back(make_token(op_it->second, two_char));
                } else {
                    tokens_.push_back(make_token(it->second, std::string(1, current)));
                }
            }
            else {
                tokens_.push_back(make_token(TokenType::UNKNOWN, std::string(1, current)));
            }
        }
        catch (std::runtime_error&) {
            lexer_error("Invalid character", line_, token_column);
        }
        catch (const std::exception& e) {
            lexer_error(e.what(), line_, token_column);
        }
    }

    tokens_.push_back(make_token(TokenType::END_OF_FILE, ""));
    return tokens_;
}


Token Lexer::read_string_literal(char quote_char) {
    std::string lexeme;
    int start_column = column_ - 1;

    while (!is_at_end()) {
        char current = advance();

        if (current == quote_char) {
            return Token(lexeme, TokenType::STR_LIT, line_, start_column);
        }

        if (current == '\\') {
            if (is_at_end()) {
                lexer_error("Unterminated escape sequence in string", line_, column_);
            }
            switch (char escaped = advance()) {
                case 'n': lexeme += '\n'; break;
                case 't': lexeme += '\t'; break;
                case 'r': lexeme += '\r'; break;
                case '\\': lexeme += '\\'; break;
                case '"': lexeme += '"'; break;
                case '\'': lexeme += '\''; break;
                default:
                    lexeme += '\\';
                    lexeme += escaped;
                    break;
            }
        }
        else if (current == '\n') {
            line_++;
            column_ = 1;
            lexeme += current;
        }
        else {
            lexeme += current;
        }
    }

    lexer_error("Unterminated string literal", line_, start_column);
}

Token Lexer::read_identifier() {
    std::string lexeme;
    int start_column = column_;

    while (!is_at_end() && (std::isalnum(peek()) || peek() == '_')) {
        lexeme += advance();
    }

    if (lexeme.empty() && !is_at_end()) {
        lexeme += advance();
    }

    TokenType type = classify_identifier(lexeme);
    return Token(lexeme, type, line_, start_column);
}

Token Lexer::read_number() {
    std::string lexeme;
    int start_column = column_;
    bool has_dot = false;
    TokenType type = TokenType::INT_LIT;

    while (!is_at_end()) {
        char current = peek();

        if (std::isdigit(current)) {
            lexeme += advance();
        }
        else if (current == '.' && !has_dot) {
            has_dot = true;
            type = TokenType::FLOAT_LIT;
            lexeme += advance();
        }
        else {
            break;
        }
    }

    if (lexeme.empty() || lexeme == ".") {
        lexer_error("Invalid number format", line_, start_column);
    }

    return Token(lexeme, type, line_, start_column);
}

char Lexer::peek() const {
    return static_cast<char>(file_.peek());
}

char Lexer::advance() {
    char ch = static_cast<char>(file_.get());
    if (ch == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return ch;
}

bool Lexer::is_at_end() const {
    return file_.eof() || file_.peek() == EOF;
}

void Lexer::skip_whitespace() {
    while (!is_at_end() && std::isspace(peek())) {
        advance();
    }
}

Token Lexer::make_token(TokenType type, std::string lexeme) const {
    return Token(std::move(lexeme), type, line_, column_ - lexeme.length());
}

TokenType Lexer::classify_identifier(const std::string& lexeme) {
    auto it = keywords_.find(lexeme);
    return it != keywords_.end() ? it->second : TokenType::IDENTIFIER;
}

void Lexer::print_tokens() const {
    for (const auto& token : tokens_) {
        std::cout << "Line " << token.line << ", Col " << token.column
                  << ": ' Lexeme: " << token.lexeme << "\t TokenType: ' -> "
                  << static_cast<int>(token.type) << std::endl;
    }
}
