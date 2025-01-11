#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "ast.hpp"
#include "semantic.hpp"
#include "codegen.hpp"

extern FILE *yyin;
extern int yyparse();
std::shared_ptr<ProgramNode> program;

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

    semanticAnalyzer.analyze(program.get());
    CodeGenerator codeGenerator(std::string(argv[2]) + ".st");
    codeGenerator.generateCode(program.get());

    std::cout << "Compilation successful. Output written to output.stack" << std::endl;
    
    fclose(yyin);

    return 0;
}