#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include "ast.hpp"
#include <iostream>

class CodeGenerator {
public:
    void generate(Program* program)
    {
        for (auto stmt : program->statements)
        {
            generateStatement(stmt);
        }
    }

private:
    void generateStatement(Statement* stmt)
    {
        if (auto letStmt = dynamic_cast<LetStatement*>(stmt))
        {
            generateExpression(letStmt->value);
            std::cout << "STORE " << letStmt->identifier << std::endl;
        } else if (auto ifStmt = dynamic_cast<IfStatement*>(stmt)) {
            generateExpression(ifStmt->condition);
            std::cout << "JZ ELSE" << std::endl;
            generateStatement(ifStmt->consequence);
            std::cout << "JMP ENDIF" << std::endl;
            std::cout << "ELSE:" << std::endl;
            if (ifStmt->alternative) {
                generateStatement(ifStmt->alternative);
            }
            std::cout << "ENDIF:" << std::endl;
        } else if (auto whileStmt = dynamic_cast<WhileStatement*>(stmt)) {
            std::cout << "WHILE:" << std::endl;
            generateExpression(whileStmt->condition);
            std::cout << "JZ ENDWHILE" << std::endl;
            generateStatement(whileStmt->body);
            std::cout << "JMP WHILE" << std::endl;
            std::cout << "ENDWHILE:" << std::endl;
        } else if (auto returnStmt = dynamic_cast<ReturnStatement*>(stmt)) {
            generateExpression(returnStmt->value);
            std::cout << "RET" << std::endl;
        }
    }

    void generateExpression(Expression* expr) {
        if (auto binExpr = dynamic_cast<BinaryExpression*>(expr)) {
            generateExpression(binExpr->left);
            generateExpression(binExpr->right);
            std::cout << "OP " << binExpr->op << std::endl;
        } else if (auto ident = dynamic_cast<Identifier*>(expr)) {
            std::cout << "LOAD " << ident->name << std::endl;
        } else if (auto num = dynamic_cast<Number*>(expr)) {
            std::cout << "PUSH " << num->value << std::endl;
        } else if (auto funcCall = dynamic_cast<FunctionCall*>(expr)) {
            for (auto arg : funcCall->arguments) {
                generateExpression(arg);
            }
            std::cout << "CALL " << funcCall->identifier << std::endl;
        }
    }
};

#endif // CODEGEN_HPP