#include "ast.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Type& type)
{
    switch (type)
    {
        case Type::TYPE_INT:    return os << "int";
        case Type::TYPE_STRING: return os << "string";
        case Type::TYPE_BOOL:   return os << "bool";
        case Type::TYPE_VOID:   return os << "void";
        default:                return os << "undefined";
    }
}

std::ostream& operator<<(std::ostream& os, const BinaryOperator& op)
{
    switch (op)
    {
        case BinaryOperator::PLUS:  return os << "+";
        case BinaryOperator::MINUS: return os << "-";
        case BinaryOperator::TIMES: return os << "*";
        case BinaryOperator::EQ:    return os << "==";
        case BinaryOperator::NEQ:   return os << "!=";
        case BinaryOperator::LT:    return os << "<";
        case BinaryOperator::GT:    return os << ">";
        case BinaryOperator::LTE:   return os << "<=";
        case BinaryOperator::GTE:   return os << ">=";
        default:                    return os << "unknown";
    }
}

void ProgramNode::accept(ASTVisitor& visitor)
{
    visitor.visit(*this);
}

void BlockNode::accept(ASTVisitor& visitor)
{
    visitor.visit(*this);
}

void FunctionDefNode::accept(ASTVisitor& visitor)
{
    visitor.visit(*this);
}

void LetStatementNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void AssignStatementNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void IfStatementNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void WhileStatementNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void ReturnStatementNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void BinaryExpressionNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void FunctionCallNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void NumberNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}

void IdentifierNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(*this);
}