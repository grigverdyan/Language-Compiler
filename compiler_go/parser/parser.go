package parser

import (
    "compiler/lexer"
	"fmt"
)

// Node represents a node in the Abstract Syntax Tree (AST)
type Node interface{}

// Program represents the entire program
type Program struct {
    Statements []Node
}

// VariableDeclaration represents a variable declaration
type VariableDeclaration struct {
    Identifier string
    Type       string
    Value      Node
}

// Assignment represents an assignment statement
type Assignment struct {
    Identifier string
    Value      Node
}

// IfStatement represents an if statement
type IfStatement struct {
    Condition   Node
    Consequence Node
    Alternative Node
}

// WhileStatement represents a while loop
type WhileStatement struct {
    Condition Node
    Body      Node
}

// ForeachStatement represents a foreach loop
type ForeachStatement struct {
    Init      Node
    Condition Node
    Post      Node
    Body      Node
}

// ReturnStatement represents a return statement
type ReturnStatement struct {
    Value Node
}

// Expression represents an expression
type Expression struct {
    Left     Node
    Operator string
    Right    Node
}

// FunctionCall represents a function call
type FunctionCall struct {
    Identifier string
    Arguments  []Node
}

// Parser is responsible for parsing tokens into an AST
type Parser struct {
    tokens  []lexer.Token
    pos     int
    current lexer.Token
}

// New creates a new Parser instance
func New(tokens []lexer.Token) *Parser {
    if len(tokens) == 0 {
        return &Parser{tokens: []lexer.Token{{Type: lexer.EOF}}, pos: 0}
    }
    return &Parser{tokens: tokens, pos: 0, current: tokens[0]}
}

// parse parses the tokens into a Program AST node
func (p *Parser) Parse() (Program, error) {
    var program Program
    for p.current.Type != lexer.EOF {
        stmt, err := p.parseStatement()
        if err != nil {
            return Program{}, fmt.Errorf("parse error: %w", err)
        }
        if stmt != nil {
            program.Statements = append(program.Statements, stmt)
        }
        p.nextToken()
    }
    return program, nil
}

// parseStatement parses a single statement
func (p *Parser) parseStatement() (Node, error) {
    switch p.current.Type {
    case lexer.LET:
        return p.parseVariableDeclaration()
    case lexer.IF:
        return p.parseIfStatement()
    case lexer.WHILE:
        return p.parseWhileStatement()
    case lexer.RETURN:
        return p.parseReturnStatement()
    default:
        return p.parseExpression()
    }
}

// parseVariableDeclaration parses a variable declaration
func (p *Parser) parseVariableDeclaration() (Node, error) {
    _, err := p.expect(lexer.LET)
    if err != nil {
        return nil, err
    }
    
    ident, err := p.expect(lexer.IDENTIFIER)
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.COLON)
    if err != nil {
        return nil, err
    }
    
    typeToken, err := p.expect(lexer.IDENTIFIER)
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.EQUAL)
    if err != nil {
        return nil, err
    }
    
    value, err := p.parseExpression()
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.SEMICOLON)
    if err != nil {
        return nil, err
    }
    
    return &VariableDeclaration{
        Identifier: ident.Literal,
        Type: typeToken.Literal,
        Value: value,
    }, nil
}

// parseIfStatement parses an if statement
func (p *Parser) parseIfStatement() (Node, error) {
    _, err := p.expect(lexer.IF)
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.LPAREN)
    if err != nil {
        return nil, err
    }
    
    condition, err := p.parseExpression()
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.RPAREN)
    if err != nil {
        return nil, err
    }
    
    consequence, err := p.parseBlock()
    if err != nil {
        return nil, err
    }
    
    var alternative Node
    if p.current.Type == lexer.ELSE {
        _, err = p.expect(lexer.ELSE)
        if err != nil {
            return nil, err
        }
        alternative, err = p.parseBlock()
        if err != nil {
            return nil, err
        }
    }
    
    return &IfStatement{
        Condition: condition,
        Consequence: consequence,
        Alternative: alternative,
	}, nil
}

// parseWhileStatement parses a while loop
func (p *Parser) parseWhileStatement() (Node, error) {
    _, err := p.expect(lexer.WHILE)
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.LPAREN)
    if err != nil {
        return nil, err
    }
    
    condition, err := p.parseExpression()
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.RPAREN)
    if err != nil {
        return nil, err
    }
    
    body, err := p.parseBlock()
    if err != nil {
        return nil, err
    }
    
    return &WhileStatement{
        Condition: condition,
        Body: body,
    }, nil
}

// parseForeachStatement parses a foreach loop
func (p *Parser) parseForeachStatement() Node {
    p.expect(lexer.FOREACH)
    p.expect(lexer.LPAREN)
    init := p.parseStatement()
    p.expect(lexer.SEMICOLON)
    condition := p.parseExpression()
    p.expect(lexer.SEMICOLON)
    post := p.parseStatement()
    p.expect(lexer.RPAREN)
    body := p.parseBlock()
    return &ForeachStatement{Init: init, Condition: condition, Post: post, Body: body}
}

// parseReturnStatement parses a return statement
func (p *Parser) parseReturnStatement() Node {
    p.expect(lexer.RETURN)
    value := p.parseExpression()
    return &ReturnStatement{Value: value}
}

// parseExpression parses an expression
func (p *Parser) parseExpression() (Node, error) {
    left, err := p.parseTerm()
    if err != nil {
        return nil, err
    }
    
    for p.current.Type == lexer.PLUS || p.current.Type == lexer.MINUS {
        operator := p.current.Literal
        p.nextToken()
        
        right, err := p.parseTerm()
        if err != nil {
            return nil, err
        }
        
        left = &Expression{
            Left: left,
            Operator: operator,
            Right: right,
        }
    }
    
    return left, nil
}

// parseTerm parses a term (identifier, number, or expression in parentheses)
func (p *Parser) parseTerm() (Node, error) {
    if p.current.Type == lexer.IDENTIFIER {
        identifier := p.current.Literal
        p.nextToken()
        if p.current.Type == lexer.LPAREN {
            return p.parseFunctionCall(identifier)
        }
        return &VariableDeclaration{Identifier: identifier}, nil
    } else if p.current.Type == lexer.NUMBER {
        value := p.current.Literal
        p.nextToken()
        return &VariableDeclaration{Value: value}, nil
    } else if p.current.Type == lexer.LPAREN {
        tok, err := p.expect(lexer.LPAREN)
        if err != nil {
            return nil, err
        }
        expr, err := p.parseExpression()
        if err != nil {
            return nil, err
        }
        _, err = p.expect(lexer.RPAREN)
        if err != nil {
            return nil, err
        }
        return expr, nil
    }
    return nil, fmt.Errorf("unexpected token: %v at line %d, column %d", 
        p.current.Type, p.current.Line, p.current.Column)
}

// parseFunctionCall parses a function call
func (p *Parser) parseFunctionCall(identifier string) Node {
    p.expect(lexer.LPAREN)
    var arguments []Node
    if p.current.Type != lexer.RPAREN {
        arguments = append(arguments, p.parseExpression())
        for p.current.Type == lexer.COMMA {
            p.expect(lexer.COMMA)
            arguments = append(arguments, p.parseExpression())
        }
    }
    p.expect(lexer.RPAREN)
    return &FunctionCall{Identifier: identifier, Arguments: arguments}
}

// parseBlock parses a block of statements
func (p *Parser) parseBlock() (Node, error) {
    _, err := p.expect(lexer.LBRACE)
    if err != nil {
        return nil, err
    }
    
    var statements []Node
    for p.current.Type != lexer.RBRACE {
        stmt, err := p.parseStatement()
        if err != nil {
            return nil, err
        }
        statements = append(statements, stmt)
        
        if p.current.Type == lexer.EOF {
            return nil, fmt.Errorf("unexpected EOF, expected '}'")
        }
    }
    
    _, err = p.expect(lexer.RBRACE)
    if err != nil {
        return nil, err
    }
    
    return statements, nil
}

// expect checks if the current token matches the expected type and advances to the next token
func (p *Parser) expect(tokenType lexer.TokenType) (lexer.Token, error) {
    if p.current.Type != tokenType {
        return lexer.Token{}, fmt.Errorf("unexpected token: expected %v, got %v at line %d, column %d", 
            tokenType, p.current.Type, p.current.Line, p.current.Column)
    }
    token := p.current
    p.nextToken()
    return token, nil
}

// nextToken advances to the next token
func (p *Parser) nextToken() {
    p.pos++
    if p.pos < len(p.tokens) {
        p.current = p.tokens[p.pos]
    } else {
        p.current = lexer.Token{Type: lexer.EOF}
    }
}

func (p *Parser) parseFunctionDeclaration() (Node, error) {
    _, err := p.expect(lexer.DEF)
    if err != nil {
        return nil, err
    }
    
    ident, err := p.expect(lexer.IDENTIFIER)
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.LPAREN)
    if err != nil {
        return nil, err
    }
    
    params := []VariableDeclaration{}
    if p.current.Type != lexer.RPAREN {
        for {
            paramName, err := p.expect(lexer.IDENTIFIER)
            if err != nil {
                return nil, err
            }
            
            _, err = p.expect(lexer.COLON)
            if err != nil {
                return nil, err
            }
            
            paramType, err := p.expect(lexer.IDENTIFIER)
            if err != nil {
                return nil, err
            }
            
            params = append(params, VariableDeclaration{
                Identifier: paramName.Literal,
                Type: paramType.Literal,
            })
            
            if p.current.Type != lexer.COMMA {
                break
            }
            p.nextToken()
        }
    }
    
    _, err = p.expect(lexer.RPAREN)
    if err != nil {
        return nil, err
    }
    
    _, err = p.expect(lexer.COLON)
    if err != nil {
        return nil, err
    }
    
    returnType, err := p.expect(lexer.IDENTIFIER)
    if err != nil {
        return nil, err
    }
    
    body, err := p.parseBlock()
    if err != nil {
        return nil, err
    }
    
    return &FunctionDeclaration{
        Name: ident.Literal,
        Parameters: params,
        ReturnType: returnType.Literal,
        Body: body,
    }, nil
}