#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include "ast.hpp"
#include <unordered_map>
#include <string>
#include <stdexcept>

class SemanticAnalyzer {
public:
    void analyze(Program* program) {
        for (auto stmt : program->statements) {
            analyzeStatement(stmt);
        }
    }

private:
    std::unordered_map<std::string, std::string> symbolTable;

    void analyzeStatement(Statement* stmt) {
        if (auto letStmt = dynamic_cast<LetStatement*>(stmt)) {
            if (symbolTable.find(letStmt->identifier) != symbolTable.end()) {
                throw std::runtime_error("Variable already declared: " + letStmt->identifier);
            }
            symbolTable[letStmt->identifier] = letStmt->type;
            analyzeExpression(letStmt->value);
        } else if (auto ifStmt = dynamic_cast<IfStatement*>(stmt)) {
            analyzeExpression(ifStmt->condition);
            analyzeStatement(ifStmt->consequence);
            if (ifStmt->alternative) {
                analyzeStatement(ifStmt->alternative);
            }
        } else if (auto whileStmt = dynamic_cast<WhileStatement*>(stmt)) {
            analyzeExpression(whileStmt->condition);
            analyzeStatement(whileStmt->body);
        } else if (auto returnStmt = dynamic_cast<ReturnStatement*>(stmt)) {
            analyzeExpression(returnStmt->value);
        }
    }

    void analyzeExpression(Expression* expr) {
        if (auto binExpr = dynamic_cast<BinaryExpression*>(expr)) {
            analyzeExpression(binExpr->left);
            analyzeExpression(binExpr->right);
        } else if (auto ident = dynamic_cast<Identifier*>(expr)) {
            if (symbolTable.find(ident->name) == symbolTable.end()) {
                throw std::runtime_error("Undeclared variable: " + ident->name);
            }
        } else if (auto num = dynamic_cast<Number*>(expr)) {
            // No semantic checks for numbers
        } else if (auto funcCall = dynamic_cast<FunctionCall*>(expr)) {
            for (auto arg : funcCall->arguments) {
                analyzeExpression(arg);
            }
        }
    }
};

#endif // SEMANTIC_HPP