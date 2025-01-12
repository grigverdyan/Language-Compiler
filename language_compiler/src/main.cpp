#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "ast.hpp"
#include "semantic.hpp"
#include "codegen.hpp"

extern FILE *yyin;
extern int yyparse();
extern ASTNode* parse_res;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << " <output_file>"<< std::endl;
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        std::cerr << "Error: Could not open input file: " << argv[1] << std::endl;
        return 1;
    }

    if (yyparse() != 0)
    {
        std::cerr << "Parsing failed." << std::endl;
        return 1;
    }

    if (dynamic_cast<ProgramNode*>(parse_res))
    {
        std::unique_ptr<ProgramNode> program(dynamic_cast<ProgramNode*>(parse_res));
        semanticAnalyzer.analyze(program.get());
        CodeGenerator codeGenerator("output.stack");
        codeGenerator.generateCode(program.get());

        std::cout << "Compilation successful. Output written to output.stack" << std::endl;
    }
    else
    {
        std::cerr << "Program is not valid AST node" << std::endl;
        fclose(yyin);
        exit(EXIT_FAILURE);
    }
    
    fclose(yyin);

    return 0;
}