#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include "ast.hpp"
#include <string>
#include <fstream>
#include <vector>

class CodeGenerator : public ASTVisitor
{
public:
    std::ofstream outputFile;
    int labelCounter = 0;
    CodeGenerator(const std::string& filename);
    ~CodeGenerator() override;
    void generateCode(ASTNode* program);
    std::string generateLabel();

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
};

extern CodeGenerator codeGenerator;

#endif // CODEGEN_HPP