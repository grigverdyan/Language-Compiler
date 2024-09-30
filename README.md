# Language-Compiler
Language Compiler of my programming language TaTev.

Algorithmic language compiler.

TaTev will support procedural and functional programming, making it suitable for both paradigms.
TaTev Language Specification in EBNF

```
Program           = Subprogram { Subprogram }
Subprogram        = _def_ Identifier Parameter_List Body

Parameter_List     = '(' [ Identifier ':' Type { ',' Identifier ':' Type } ]
Body              = { '{' Statement '}' }

Statement         = Variable_Declaration | Assignment | Conditional_Statement | Loop_While_Statement |         
                    Loop_Foreach_Statement | Expression ';' | Return_Statement

Variable_Declaration    = _let_ Identifier ':' Type [ '=' Expression ] ';'
Assignment              = Identifier '=' Expression ';'
Conditional_Statement   = _if_ '(' Expression ')' Body { _elif_ '(' Expression ')' Body } [ _else_ Body }
Loop_While_Statement    = _while_ '(' Expression ')' Body
Loop_Foreach_Statement  = _foreach_ '(' Variable_Declaration | Assignment ';' Expression ';' Assignment ')' Body
Return_Statement        = _return_ Identifier | Expression ';'

Expression              = Simple_Expression [ Comparison_Operator Simple_Expression ]
Simple_Expression       = Term { Arithmetic_Operator Term }
Term                    = Identifier | Number | Function_Call | '(' Expression ')'

Function_Call           = Identifier '(' [ Argument_List ] ')'
Argument_List           = Expression { ',' Expression } 

Type                   = _int_ | _float_ | _boolean_ | _string_ | _void_
Identifier             = Letter { Letter | Digit }
Number                 = Digit { Digit }
Arithmetic_Operator    = '+' | '-' | '*' | '%' | '/'
Comparison_Operator    = '==' | '<' | '>' | '<=' | '>=' | '!='
Logical_operator       = '&&' | '||' | '!'

Comment                = '#'  

```


Code example:

```
# Function that adds two integers
def add(a: int, b: int): int
{
    return a + b;
}

# The main function
def main(): void
{
    let x: int = 5;
    let y: int = 10;
    let result: int = add(x, y);

    if (result > 10)
    {
        result = result * 2;
    }

    while (result > 0)
    {
        print(result);
        result = result - 1;
    }
}

```
