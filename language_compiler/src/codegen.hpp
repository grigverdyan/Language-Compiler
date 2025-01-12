#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include "ast.hpp"
#include "macros.hpp"

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class CodeGenerator : public ASTVisitor
{
    std::ofstream outputFile;
    int labelCounter = 0;

public:
    CodeGenerator(const std::string& filename)
    {
        outputFile.open(filename);
        if (!outputFile.is_open())
        {
            std::cerr << RED << "Error: Could not open output file: " << YELLOW << filename << RESET << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    ~CodeGenerator() { outputFile.close(); }

    void generateCode(ASTNode* program) { program->accept(*this); }
    
    std::string generateLabel() { return "label" + std::to_string(labelCounter++); }

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