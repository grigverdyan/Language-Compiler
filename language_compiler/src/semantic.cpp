#include "semantic.hpp"
#include "macros.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>

SemanticAnalyzer semanticAnalyzer;

void SemanticAnalyzer::enterScope()
{
    currentScope = new SymbolTable(currentScope);

    // For Debugging info
#ifdef DEBUG
    std::cerr << "Entering scope, current scope: " << currentScope << std::endl;
    // End Debugging info
#endif
}

void SemanticAnalyzer::exitScope()
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Exiting scope, current scope: " << currentScope << std::endl;
    // End Debugging info
#endif    
    
    delete currentScope;
    currentScope = currentScope->parent;
}

// Private member functions
void SemanticAnalyzer::error(const std::string& message)
{
    std::cerr << RED << "Semantic Error: " << message << RESET << std::endl;
    exit(1);
}
void SemanticAnalyzer::checkType(ASTNode* node, Type expectedType, const std::string& errorMessage)
{
    Type actualType = checkExpressionType(node);
        if (actualType != expectedType && expectedType != Type::TYPE_UNDEFINED)
        {
            error(errorMessage + " Expected " +  std::string(1, static_cast<char>(expectedType))  + ", but got " + std::string(1, static_cast<char>(actualType)));
        }
}
Type SemanticAnalyzer::checkExpressionType(ASTNode* node)
{
    if (dynamic_cast<NumberNode*>(node))
    {
        return Type::TYPE_INT;
    }
    else if (dynamic_cast<IdentifierNode*>(node))
    {
        IdentifierNode* idNode = dynamic_cast<IdentifierNode*>(node);
        Type type = currentScope->getSymbolType(idNode->name);
        if (type == Type::TYPE_UNDEFINED)
        {
            error("Undeclared variable: " + idNode->name);
        }
        return type;
    }
    else if (dynamic_cast<BinaryExpressionNode*>(node))
    {
        BinaryExpressionNode* binNode = dynamic_cast<BinaryExpressionNode*>(node);
        Type leftType = checkExpressionType(binNode->left.get());
        Type rightType = checkExpressionType(binNode->right.get());
        return inferBinaryOperationType(binNode->op, leftType, rightType);
    }
    else if (dynamic_cast<FunctionCallNode*>(node))
    {
        FunctionCallNode* callNode = dynamic_cast<FunctionCallNode*>(node);
        return getFunctionReturnType(callNode->name);
    }
    return Type::TYPE_UNDEFINED;
}

Type SemanticAnalyzer::inferBinaryOperationType(BinaryOperator op, Type leftType, Type rightType)
{
    if (leftType == Type::TYPE_UNDEFINED || rightType == Type::TYPE_UNDEFINED)
        return Type::TYPE_UNDEFINED;
    if (op == BinaryOperator::PLUS || op == BinaryOperator::MINUS || op == BinaryOperator::TIMES)
    {
        if (leftType == Type::TYPE_INT && rightType == Type::TYPE_INT)
            return Type::TYPE_INT;
        else 
        {
            error("Type mismatch in binary operation. Expected both types to be int");
            return Type::TYPE_UNDEFINED;
        }
    } 
    else if ( op == BinaryOperator::EQ || op == BinaryOperator::NEQ 
                || op == BinaryOperator::LT || op == BinaryOperator::GT 
                || op == BinaryOperator::LTE || op == BinaryOperator::GTE)
    {
        if (leftType == rightType) 
            return Type::TYPE_BOOL;
        else
        {
            error("Type mismatch in comparison operation.");
            return Type::TYPE_UNDEFINED;
        }
    }
    return Type::TYPE_UNDEFINED;
}

Type SemanticAnalyzer::getFunctionReturnType(const std::string& name)
{
    FunctionDefNode* func = getFunctionDefinition(name);
    if (func)
        return func->returnType;
    return Type::TYPE_UNDEFINED;
}

FunctionDefNode* SemanticAnalyzer::getFunctionDefinition(const std::string& name)
{
    if (functions.find(name) != functions.end())
    {
        return functions[name];
    }
    return nullptr;
}

void SemanticAnalyzer::visit(ProgramNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting ProgramNode" << std::endl;
    for (const auto& funcPair : functions)
    {
        std::cerr << "Function found: " << funcPair.first << " : " << funcPair.second << std::endl;
    }
    // End Debugging info
#endif

    std::set<ASTNode*> visitedSubprograms;
    for (const auto& subprogram : node.subprograms)
    {
        if(visitedSubprograms.find(subprogram.get()) == visitedSubprograms.end())
        {
            subprogram->accept(*this);
            visitedSubprograms.insert(subprogram.get());
        }
    }
}

void SemanticAnalyzer::visit(BlockNode& node)
{
    enterScope();
    for (const auto& stmt : node.statements)
    {
        stmt->accept(*this);
    }
    exitScope();
}

void SemanticAnalyzer::visit(FunctionDefNode& node)
{
     if (functions.find(node.name) != functions.end())
     {
           error("Function " + node.name + " already defined.");
           return;
     }

    functions[node.name] = &node;

    // For Debugging info 
#ifdef DEBUG   
    std::cerr << "Visiting FunctionDefNode: " << node.name << ", current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    currentFunction = &node;

    enterScope();
    for(const auto& param : node.parameters)
    {
        currentScope->addSymbol(param.first, param.second);
    }

    node.body->accept(*this);
    exitScope();
    currentFunction = nullptr;
}

void SemanticAnalyzer::visit(LetStatementNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting LetStatementNode: " << node.varName << ", current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    if(node.initExpr)
        checkType(node.initExpr.get(), node.varType, "Type mismatch in variable initialization for " + node.varName + ".");
   currentScope->addSymbol(node.varName, node.varType);
}

void SemanticAnalyzer::visit(AssignStatementNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting AssignStatementNode: " << node.varName << ", current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    Type varType = currentScope->getSymbolType(node.varName);
    if (varType == Type::TYPE_UNDEFINED)
    {
        error("Assignment to undeclared variable: " + node.varName);
    }
    checkType(node.expression.get(), varType, "Type mismatch in assignment to variable " + node.varName + ".");
}

void SemanticAnalyzer::visit(IfStatementNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting IfStatementNode, current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    checkType(node.condition.get(), Type::TYPE_BOOL, "If condition must be a boolean expression.");
    node.body->accept(*this);
    for(const auto& elif : node.elifClauses)
    {
         checkType(elif.first, Type::TYPE_BOOL, "Elif condition must be a boolean expression.");
         elif.second->accept(*this);
    }
    if(node.elseClause)
        node.elseClause->accept(*this);
}

void SemanticAnalyzer::visit(WhileStatementNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting WhileStatementNode, current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    checkType(node.condition.get(), Type::TYPE_BOOL, "While condition must be a boolean expression.");
    node.body->accept(*this);
}

void SemanticAnalyzer::visit(ReturnStatementNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting ReturnStatementNode, current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    if (currentFunction == nullptr)
    {
        error("Return statement outside a function.");
        return;
    }
    checkType(node.expression.get(), currentFunction->returnType, "Return type does not match function return type.");
    node.expression->accept(*this);
}

void SemanticAnalyzer::visit(BinaryExpressionNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting BinaryExpressionNode, current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    checkExpressionType(&node);
    node.left->accept(*this);
    node.right->accept(*this);
}

void SemanticAnalyzer::visit(FunctionCallNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting FunctionCallNode: " << node.name << ", current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    FunctionDefNode* func = getFunctionDefinition(node.name);
    if (!func)
        error("Call to undeclared function " + node.name);

    if (func->parameters.size() != node.arguments.size())
    {
        error("Incorrect number of arguments for function " + node.name);
    }

    for (size_t i = 0; i < node.arguments.size(); ++i)
    {
        checkType(node.arguments[i].get(), func->parameters[i].second, "Type mismatch in argument " + std::to_string(i+1) + " of function " + node.name);
    }
}

void SemanticAnalyzer::visit(NumberNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting NumberNode: " << node.value << ", current scope: " << currentScope << std::endl;
    // End Debugging info
#endif
}

void SemanticAnalyzer::visit(IdentifierNode& node)
{
    // For Debugging info
#ifdef DEBUG
    std::cerr << "Visiting IdentifierNode: " << node.name << ", current scope: " << currentScope << std::endl;
    // End Debugging info
#endif

    if (currentScope->getSymbolType(node.name) == Type::TYPE_UNDEFINED && getFunctionDefinition(node.name) == nullptr)
    {
        error("Undeclared identifier: " + node.name);
    }
}

void SemanticAnalyzer::analyze(ASTNode* program)
{
    program->accept(*this);
}