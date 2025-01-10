#include <iostream>
#include <string>
#include <vector>
#include "ast.hpp"
#include "semantic.hpp"
#include "codegen.hpp"

extern int yyparse();
extern Program* program;
extern FILE* yyin;

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        FILE* file = fopen(argv[1], "r");
        if (!file)
        {
            perror(argv[1]);
            return 1;
        }
        yyin = file; 
    }

    yyparse();

    SemanticAnalyzer semanticAnalyzer;
    semanticAnalyzer.analyze(program);

    CodeGenerator codeGenerator;
    codeGenerator.generate(program);

    return 0;
}