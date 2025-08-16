#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

enum TokenType {
    // Keywords
    KEY_PRINT, KEY_IF, KEY_ELSE, KEY_READ, KEY_WHILE, KEY_FOR,
    KEY_FUNCTION, KEY_VAR, KEY_RETURN, KEY_TRUE, KEY_FALSE,

    // Delimiters
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, SEMICOLON, COLON,

    // Operators
    ADD_OP, SUB_OP, MUL_OP, DIV_OP, MOD_OP, POW_OP,
    AND_OP, OR_OP, NOT_OP, INT_DIV_OP,
    ASSIGN_OP, EQUAL_OP, NOT_EQUAL_OP,
    GREATER_OP, GEQUAL_OP, LESSER_OP, LEQUAL_OP,

    // Parameters
    PARAM_NAME, PARAM_INT, PARAM_FLOAT, PARAM_STRING, PARAM_BOOL, PARAM_CHAR, RETURN_TYPE,

    // Data types
    DATATYPE_INT, DATATYPE_FLOAT, DATATYPE_STRING, DATATYPE_BOOL, DATATYPE_CHAR,

    // Literals / identifiers
    IDENTIFIER, INT_LIT, FLOAT_LIT, STR_LIT,

    // Special
    END_OF_FILE, UNKNOWN
};

struct Token {
    std::string lexeme;
    TokenType type;
    int line;
    int column;

    Token(
        std::string lex = "",
        TokenType t = TokenType::UNKNOWN,
        int l = 1,
        int c = 1)
        : lexeme(std::move(lex)), type(t), line(l), column(c) {}
};


class Lexer {
public:
    explicit Lexer(const std::string& filename);

    std::vector<Token> tokenize();
    void print_tokens() const;
    [[noreturn]] static void lexer_error(const std::string& msg, int line, int column);
    const std::vector<Token>& get_tokens() const { return tokens_; }

private:
    void skip_whitespace();
    Token read_string_literal(char quote_char);
    Token read_identifier();
    Token read_number();

    char peek() const;
    char advance();
    bool is_at_end() const;
    Token make_token(TokenType type, const std::string &lexeme) const;

    static TokenType classify_identifier(const std::string& lexeme);

    mutable std::ifstream file_;        // input file stream
    std::vector<Token> tokens_;         // holds all the tokens
    int line_ = 1;                      // line things start
    int column_ = 1;

    // static lookup tables
    static const std::unordered_map<std::string, TokenType> keywords_;
    static const std::unordered_map<std::string, TokenType> operators_;
    static const std::unordered_map<char, TokenType> single_char_tokens_;
};
