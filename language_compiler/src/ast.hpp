#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class Expression : public ASTNode {
};

class Statement : public ASTNode {
};

class Program : public ASTNode {
public:
    std::vector<Statement*> statements;
};

class LetStatement : public Statement {
public:
    std::string identifier;
    std::string type;
    Expression* value;
};

class IfStatement : public Statement {
public:
    Expression* condition;
    Statement* consequence;
    Statement* alternative;
};

class WhileStatement : public Statement {
public:
    Expression* condition;
    Statement* body;
};

class ReturnStatement : public Statement {
public:
    Expression* value;
};

class FunctionCall : public Expression {
public:
    std::string identifier;
    std::vector<Expression*> arguments;
};

class BinaryExpression : public Expression {
public:
    Expression* left;
    std::string op;
    Expression* right;
};

class Identifier : public Expression {
public:
    std::string name;
};

class Number : public Expression {
public:
    int value;
};

class BlockStatement : public Statement {
public:
    std::vector<Statement*> statements;
};

class FunctionDefinition : public Statement {
public:
    std::string name;
    std::vector<std::pair<std::string, std::string>> parameters;
    std::string returnType;
    std::vector<Statement*> body;
};

#endif // AST_HPP