#include "codegen.hpp"
#include <iostream>
#include <sstream>

void CodeGenerator::visit(ProgramNode& node)
{
    for (const auto& subprogram : node.subprograms)
    {
        subprogram->accept(*this);
    }
}

void CodeGenerator::visit(BlockNode& node)
{
    for (const auto& stmt : node.statements)
    {
        stmt->accept(*this);
    }
}

void CodeGenerator::visit(FunctionDefNode& node)
{
    outputFile << node.name << ":" << std::endl;
    for(const auto& param : node.parameters)
    {
        outputFile << "push " << param.first << std::endl;
    }
    node.body->accept(*this);
    outputFile << "ret" << std::endl << std::endl;
}

void CodeGenerator::visit(LetStatementNode& node)
{
    if (node.initExpr)
    {
        node.initExpr->accept(*this);
    }
    else
    {
        outputFile << "push 0" << std::endl; // Default to 0
    }

    outputFile << "pop " << node.varName << std::endl;
}

void CodeGenerator::visit(AssignStatementNode& node)
{
    node.expression->accept(*this);
    outputFile << "pop " << node.varName << std::endl;
}

void CodeGenerator::visit(IfStatementNode& node)
{
    std::string elseLabel = generateLabel();
    std::string endIfLabel = generateLabel();

    node.condition->accept(*this);
    outputFile << "jz " << elseLabel << std::endl;
    node.body->accept(*this);
    outputFile << "jmp " << endIfLabel << std::endl;
    outputFile << elseLabel << ":" << std::endl;
    for (const auto& elifClause : node.elifClauses)
    {
        std::string nextElifLabel = generateLabel();
        elifClause.first->accept(*this);
        outputFile << "jz " << nextElifLabel << std::endl;
        elifClause.second->accept(*this);
        outputFile << "jmp " << endIfLabel << std::endl;
        outputFile << nextElifLabel << ":" << std::endl;

    }
    if (node.elseClause)
    {
        node.elseClause->accept(*this);
    }

    outputFile << endIfLabel << ":" << std::endl;
}

void CodeGenerator::visit(WhileStatementNode& node)
{
    std::string loopStart = generateLabel();
    std::string loopEnd = generateLabel();

    outputFile << loopStart << ":" << std::endl;
    node.condition->accept(*this);
    outputFile << "jz " << loopEnd << std::endl;
    node.body->accept(*this);
    outputFile << "jmp " << loopStart << std::endl;
    outputFile << loopEnd << ":" << std::endl;
}

void CodeGenerator::visit(ReturnStatementNode& node)
{
    node.expression->accept(*this);
}

void CodeGenerator::visit(BinaryExpressionNode& node)
{
    node.left->accept(*this);
    node.right->accept(*this);
    switch(node.op) 
    {
        case BinaryOperator::PLUS:  outputFile << "add" << std::endl;   break;
        case BinaryOperator::MINUS: outputFile << "sub" << std::endl;   break;
        case BinaryOperator::TIMES: outputFile << "mul" << std::endl;   break;
        case BinaryOperator::EQ:    outputFile << "eq" << std::endl;    break;
        case BinaryOperator::NEQ:   outputFile << "neq" << std::endl;   break;
        case BinaryOperator::LT:    outputFile << "lt" << std::endl;    break;
        case BinaryOperator::GT:    outputFile << "gt" << std::endl;    break;
        case BinaryOperator::LTE:   outputFile << "lte" << std::endl;   break;
        case BinaryOperator::GTE:   outputFile << "gte" << std::endl;   break;
    }
}

void CodeGenerator::visit(FunctionCallNode& node)
{
    for (const auto& arg : node.arguments)
    {
         arg->accept(*this);
    }
    outputFile << "call " << node.name << std::endl;
}

void CodeGenerator::visit(NumberNode& node)
{
    outputFile << "push " << node.value << std::endl;
}

void CodeGenerator::visit(IdentifierNode& node)
{
    outputFile << "push " << node.name << std::endl;
}