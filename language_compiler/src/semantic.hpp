#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include "ast.hpp"
#include <map>
#include <string>
#include <iostream>
#include <stack>

class SymbolTable
{
public:
    std::map<std::string, Type> symbols;
    SymbolTable* parent;
    bool isFunctionScope;

    SymbolTable(SymbolTable* parent = nullptr) 
        : parent(parent)
        , isFunctionScope(false) 
    {};

    void addSymbol(const std::string& name, Type type)
    {
        if (symbols.find(name) != symbols.end())
        {
            std::cerr << "Semantic Error: Variable '" << name << "' already declared in this scope." << std::endl;
            exit(1);
        }
        symbols[name] = type;
    }
    Type getSymbolType(const std::string& name)
    {
        if (symbols.find(name) != symbols.end())
            return symbols[name];

        if (parent != nullptr)
            return parent->getSymbolType(name);

        return Type::TYPE_UNDEFINED;
    }
};

class SemanticAnalyzer : public ASTVisitor
{
    SymbolTable globalSymbolTable;
    SymbolTable* currentScope;
    std::map<std::string, FunctionDefNode*> functions;
    FunctionDefNode* currentFunction = nullptr;

public:
    SemanticAnalyzer()
        : currentScope(&globalSymbolTable)
    {}
    ~SemanticAnalyzer() = default;
    
    void enterScope();
    void exitScope();

    void visit(ProgramNode& node) override;
    void visit(BlockNode& node) override;
    void visit(FunctionDefNode& node) override;
    void visit(LetStatementNode& node) override;
    void visit(AssignStatementNode& node) override;
    void visit(IfStatementNode& node) override;
    void visit(WhileStatementNode& node) override;
    void visit(ReturnStatementNode& node) override;
    void visit(BinaryExpressionNode& node) override;
    void visit(FunctionCallNode& node) override;
    void visit(NumberNode& node) override;
    void visit(IdentifierNode& node) override;
    void analyze(ASTNode* program);

private:
    void error(const std::string& message);
    void checkType(ASTNode* node, Type expectedType, const std::string& errorMessage);
    Type inferBinaryOperationType(BinaryOperator op, Type leftType, Type rightType);
    Type getFunctionReturnType(const std::string& name);
    Type checkExpressionType(ASTNode* node);
    FunctionDefNode* getFunctionDefinition(const std::string& name);
};

extern SemanticAnalyzer semanticAnalyzer;

#endif // SEMANTIC_HPP