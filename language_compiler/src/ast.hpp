#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <variant>

enum class Type
{
    TYPE_INT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VOID,
    TYPE_UNDEFINED
};

std::ostream& operator<<(std::ostream& os, const Type& type);

enum class BinaryOperator
{
    PLUS,   // +    
    MINUS,  // -
    TIMES,  // *
    EQ,     // ==
    NEQ,    // !=
    LT,     // <
    GT,     // >
    LTE,    // <=
    GTE     // >=
};

std::ostream& operator<<(std::ostream& os, const BinaryOperator& op);

// Forward declaration
class ASTVisitor;

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

class ProgramNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<ASTNode>> subprograms;

    ProgramNode(ASTNode* subprogram = nullptr)
    {
        if(subprogram) 
            subprograms.push_back(std::unique_ptr<ASTNode>(subprogram));
    }

    void accept(ASTVisitor& visitor) override;
};


class BlockNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    BlockNode(std::vector<ASTNode*> stmts)
    {
        for(auto stmt : stmts) 
            statements.push_back(std::unique_ptr<ASTNode>(stmt));
    }

    void accept(ASTVisitor& visitor) override;
};

class FunctionDefNode : public ASTNode
{
public:
    std::string name;
    std::vector<std::pair<std::string, Type>> parameters;
    Type returnType;
    std::unique_ptr<ASTNode> body;

    FunctionDefNode(const std::string& name, const std::vector<std::pair<std::string, Type>>& params, Type returnType, ASTNode* body)
        : name(name)
        , parameters(params)
        , returnType(returnType)
        , body(std::unique_ptr<ASTNode>(body))
    {}

    void accept(ASTVisitor& visitor) override;
};

class LetStatementNode : public ASTNode
{
public:
    std::string varName;
    Type varType;
    std::unique_ptr<ASTNode> initExpr;

    LetStatementNode(const std::string& name, Type type, ASTNode* expr = nullptr)
        : varName(name)
        , varType(type)
        , initExpr(std::unique_ptr<ASTNode>(expr))
    {}

    void accept(ASTVisitor& visitor) override;
};

class AssignStatementNode : public ASTNode
{
public:
    std::string varName;
    std::unique_ptr<ASTNode> expression;

    AssignStatementNode(const std::string& name, ASTNode* expr)
        : varName(name)
        , expression(std::unique_ptr<ASTNode>(expr))
    {}

    void accept(ASTVisitor& visitor) override;
};

class IfStatementNode : public ASTNode
{
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;
    std::vector<std::pair<ASTNode*, ASTNode*>> elifClauses;
    ASTNode* elseClause;

    IfStatementNode(ASTNode* cond, ASTNode* body, std::vector<std::pair<ASTNode*, ASTNode*>> elifs = {}, ASTNode* elseClause = nullptr)
        : condition(std::unique_ptr<ASTNode>(cond))
        , body(std::unique_ptr<ASTNode>(body))
        , elifClauses(elifs)
        , elseClause(elseClause)
    {}
    
    ~IfStatementNode()
    {
        for(const auto& pair : elifClauses)
        {
            delete pair.first;
            delete pair.second;
        }
        if(elseClause) 
            delete elseClause;
    }
    void accept(ASTVisitor& visitor) override;
};

class WhileStatementNode : public ASTNode
{
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;

    WhileStatementNode(ASTNode* cond, ASTNode* body) 
        : condition(std::unique_ptr<ASTNode>(cond))
        , body(std::unique_ptr<ASTNode>(body))
    {}

    void accept(ASTVisitor& visitor) override;
};

class ReturnStatementNode : public ASTNode
{
public:
    std::unique_ptr<ASTNode> expression;

    ReturnStatementNode(ASTNode* expr) 
        : expression(std::unique_ptr<ASTNode>(expr))
    {}

    void accept(ASTVisitor& visitor) override;
};

class BinaryExpressionNode : public ASTNode
{
public:
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    BinaryOperator op;

    BinaryExpressionNode(ASTNode* left, BinaryOperator op, ASTNode* right)
        : left(std::unique_ptr<ASTNode>(left))
        , op(op)
        , right(std::unique_ptr<ASTNode>(right))
    {}
    
    void accept(ASTVisitor& visitor) override;
};

class FunctionCallNode : public ASTNode
{
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    FunctionCallNode(const std::string& name, const std::vector<ASTNode*>& args)
        : name(name)
    {
        for(auto arg : args) 
            arguments.push_back(std::unique_ptr<ASTNode>(arg));
    }

    void accept(ASTVisitor& visitor) override;
};

class NumberNode : public ASTNode
{
public:
    int value;
    NumberNode(int val) 
        : value(val) 
    {}

    void accept(ASTVisitor& visitor) override;
};

class IdentifierNode : public ASTNode
{
public:
    std::string name;

    IdentifierNode(const std::string& name)
        : name(name)
    {}

    void accept(ASTVisitor& visitor) override;
};


// Visitor Pattern
class ASTVisitor
{
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(BlockNode& node) = 0;
    virtual void visit(FunctionDefNode& node) = 0;
    virtual void visit(LetStatementNode& node) = 0;
    virtual void visit(AssignStatementNode& node) = 0;
    virtual void visit(IfStatementNode& node) = 0;
    virtual void visit(WhileStatementNode& node) = 0;
    virtual void visit(ReturnStatementNode& node) = 0;
    virtual void visit(BinaryExpressionNode& node) = 0;
    virtual void visit(FunctionCallNode& node) = 0;
    virtual void visit(NumberNode& node) = 0;
    virtual void visit(IdentifierNode& node) = 0;
};

#endif // AST_HPP