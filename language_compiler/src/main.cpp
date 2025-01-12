#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "macros.hpp"
#include "ast.hpp"
#include "semantic.hpp"
#include "codegen.hpp"

extern FILE *yyin;
extern int yyparse();
extern ASTNode* parse_res;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << BLUE << "Usage: " << CYAN << argv[0] << " <input_file>" << " <output_file>" << RESET << std::endl;
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        std::cerr << RED << "Error: Could not open input file: " << argv[1] << RESET << std::endl;
        return 1;
    }

    if (yyparse() != 0)
    {
        std::cerr << RED <<  "Parsing failed." << RESET << std::endl;
        return 1;
    }

    if (dynamic_cast<ProgramNode*>(parse_res))
    {
        std::unique_ptr<ProgramNode> program(dynamic_cast<ProgramNode*>(parse_res));
        semanticAnalyzer.analyze(program.get());
        CodeGenerator codeGenerator(std::string(argv[2]) + ".st");
        codeGenerator.generateCode(program.get());

        std::cout << GREEN <<"Compilation successful. Output written to " << YELLOW << argv[2] << ".st" << RESET << std::endl;
    }
    else
    {
        std::cerr << RED << "Program is not valid!" << RESET << std::endl;
        fclose(yyin);
        exit(EXIT_FAILURE);
    }
    
    fclose(yyin);

    return 0;
}