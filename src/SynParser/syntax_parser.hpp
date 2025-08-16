#pragma once
#include "../Lexer/lexer.hpp"
#include <string>
#include <vector>
#include <memory>

// === Node Type Identification ===
enum class NodeType {
    Program,
    Function,
    Parameter,
    Declaration,
    Assignment,
    If,
    While,
    Return,
    Block,
    BinaryOp,
    UnaryOp,
    Literal,
    Variable,
    FunctionCall
};

// === AST Base ===
struct ASTNode {
    NodeType type;
    int line = -1;
    int column = -1;

    virtual ~ASTNode() = default;

protected:
    ASTNode(NodeType t) : type(t) {}
};
using ASTNodePTR = std::shared_ptr<ASTNode>;

// === Program ===
struct ProgramNode final : ASTNode {
    std::vector<ASTNodePTR> children; // functions, globals, etc.

    ProgramNode() : ASTNode(NodeType::Program) {}
};

// === Parameters ===
struct ParameterNode final : ASTNode {
    std::string type;
    std::string name;

    ParameterNode() : ASTNode(NodeType::Parameter) {}
    ParameterNode(const std::string& t, const std::string& n)
        : ASTNode(NodeType::Parameter), type(t), name(n) {}
};

// === Functions ===
struct FunctionNode final : ASTNode {
    std::string name;
    std::string returnType;
    std::vector<std::shared_ptr<ParameterNode>> parameters;
    std::vector<ASTNodePTR> body;

    FunctionNode() : ASTNode(NodeType::Function) {}
};

// === Statements ===
struct StatementNode : ASTNode {
protected:
    StatementNode(NodeType t) : ASTNode(t) {}
};

struct DeclarationNode final : StatementNode {
    std::string type;
    std::string name;
    ASTNodePTR initializer; // may be nullptr

    DeclarationNode() : StatementNode(NodeType::Declaration) {}
};

struct AssignmentNode final : StatementNode {
    std::string name;
    ASTNodePTR expression;

    AssignmentNode() : StatementNode(NodeType::Assignment) {}
};

struct IfNode final : StatementNode {
    ASTNodePTR condition;
    std::vector<ASTNodePTR> body;
    std::vector<ASTNodePTR> elseBody; // optional

    IfNode() : StatementNode(NodeType::If) {}
};

struct WhileNode final : StatementNode {
    ASTNodePTR condition;
    std::vector<ASTNodePTR> body;

    WhileNode() : StatementNode(NodeType::While) {}
};

struct ReturnNode final : StatementNode {
    ASTNodePTR expression; // may be nullptr for void returns

    ReturnNode() : StatementNode(NodeType::Return) {}
};

struct BlockNode final : StatementNode {
    std::vector<ASTNodePTR> statements;

    BlockNode() : StatementNode(NodeType::Block) {}
};

// === Expressions ===
struct ExpressionNode : ASTNode {
protected:
    ExpressionNode(NodeType t) : ASTNode(t) {}
};

struct BinaryOpNode final : ExpressionNode {
    std::string op;
    ASTNodePTR left;
    ASTNodePTR right;

    BinaryOpNode() : ExpressionNode(NodeType::BinaryOp) {}
    BinaryOpNode(const std::string& operation, ASTNodePTR l, ASTNodePTR r)
        : ExpressionNode(NodeType::BinaryOp), op(operation), left(l), right(r) {}
};

struct UnaryOpNode final : ExpressionNode {
    std::string op;
    ASTNodePTR operand;

    UnaryOpNode() : ExpressionNode(NodeType::UnaryOp) {}
    UnaryOpNode(const std::string& operation, ASTNodePTR operand)
        : ExpressionNode(NodeType::UnaryOp), op(operation), operand(operand) {}
};

struct LiteralNode final : ExpressionNode {
    std::string value;
    std::string literalType; // "int", "float", "string", "bool"

    LiteralNode() : ExpressionNode(NodeType::Literal) {}
    LiteralNode(const std::string& val, const std::string& type = "")
        : ExpressionNode(NodeType::Literal), value(val), literalType(type) {}
};

struct VariableNode final : ExpressionNode {
    std::string name;

    VariableNode() : ExpressionNode(NodeType::Variable) {}
    VariableNode(const std::string& n) : ExpressionNode(NodeType::Variable), name(n) {}
};

struct FunctionCallNode final : ExpressionNode {
    std::string name;
    std::vector<ASTNodePTR> arguments;

    FunctionCallNode() : ExpressionNode(NodeType::FunctionCall) {}
    FunctionCallNode(const std::string& n) : ExpressionNode(NodeType::FunctionCall), name(n) {}
};

// === Parser ===
class SyntaxParser {
public:
    SyntaxParser(const std::vector<Token> &tokens);

    ASTNodePTR parse_program();
    void print_ast(const ASTNodePTR& node, int indent = 0) const;

private:
    std::vector<Token> tokens;
    size_t current;

    // utility functions
    const Token& peek() const;          // take a gander at the next token
    const Token& advance();             // move forward
    bool check(TokenType type) const;   // check if current token matches type
    bool isAtEnd() const;               // check if we've reached the end
    void match(TokenType type);         // match tokens
    bool consume(TokenType type);       // consume token if it matches, return success
    void setSourceLocation(ASTNodePTR node, const Token& token) const; // set line/column info
    [[noreturn]] static void error(const std::string& msg, int line, int column); // output any errors

    // Grammar rules
    ASTNodePTR parse_function();
    ASTNodePTR parse_statement();
    ASTNodePTR parse_declaration();
    ASTNodePTR parse_assignment();
    ASTNodePTR parse_if();
    ASTNodePTR parse_while();
    ASTNodePTR parse_return();
    ASTNodePTR parse_block();

    // Expression parsing with precedence
    ASTNodePTR parse_expression();
    ASTNodePTR parse_logical_or();
    ASTNodePTR parse_logical_and();
    ASTNodePTR parse_equality();
    ASTNodePTR parse_comparison();
    ASTNodePTR parse_term();
    ASTNodePTR parse_factor();
    ASTNodePTR parse_unary();
    ASTNodePTR parse_primary();
    ASTNodePTR parse_function_call();

    // Helper functions
    std::shared_ptr<ParameterNode> parse_parameter();
    std::vector<std::shared_ptr<ParameterNode>> parse_parameter_list();
    std::vector<ASTNodePTR> parse_argument_list();
};
