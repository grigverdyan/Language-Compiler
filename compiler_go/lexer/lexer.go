package lexer

import (
    "unicode"
    "unicode/utf8"
)

// TokenType represents the type of a token
type TokenType string

// Define the types of tokens that can be recognized
const (
    LET       TokenType = "LET"
    IF        TokenType = "IF"
    ELIF      TokenType = "ELIF"
    ELSE      TokenType = "ELSE"
    WHILE     TokenType = "WHILE"
    FOREACH   TokenType = "FOREACH"
    RETURN    TokenType = "RETURN"
    IDENTIFIER TokenType = "IDENTIFIER"
    NUMBER    TokenType = "NUMBER"
    PLUS      TokenType = "PLUS"
    MINUS     TokenType = "MINUS"
    EQUAL     TokenType = "EQUAL"
    LPAREN    TokenType = "LPAREN"
    RPAREN    TokenType = "RPAREN"
    LBRACE    TokenType = "LBRACE"
    RBRACE    TokenType = "RBRACE"
    SEMICOLON TokenType = "SEMICOLON"
    COMMA     TokenType = "COMMA"
    INT       TokenType = "INT"
    STRING    TokenType = "STRING"
    BOOL      TokenType = "BOOL"
    VOID      TokenType = "VOID"
    COMMENT   TokenType = "COMMENT"
    EOF       TokenType = "EOF"
)

// Token represents a token with its type, literal value, line, and column
type Token struct {
    Type    TokenType
    Literal string
    Line    int
    Column  int
}

// Lexer is responsible for tokenizing the input source code
type Lexer struct {
    input  string
    pos    int
    line   int
    column int
}

// New creates a new Lexer instance
func New(input string) *Lexer {
    return &Lexer{input: input, pos: 0, line: 1, column: 1}
}

// NextToken returns the next token from the input
func (l *Lexer) NextToken() Token {
    l.skipWhitespace()

    if l.pos >= len(l.input) {
        return Token{Type: EOF, Line: l.line, Column: l.column}
    }

    ch := l.peek()

    if unicode.IsLetter(ch) || ch == '_' {
        return l.readIdentifier()
    } else if unicode.IsDigit(ch) {
        return l.readNumber()
    } else if ch == '#' {
        l.skipComment()
        return l.NextToken()
    }

    switch ch {
    case '+':
        return l.newToken(PLUS, string(ch))
    case '-':
        return l.newToken(MINUS, string(ch))
    case '=':
        return l.newToken(EQUAL, string(ch))
    case '(':
        return l.newToken(LPAREN, string(ch))
    case ')':
        return l.newToken(RPAREN, string(ch))
    case '{':
        return l.newToken(LBRACE, string(ch))
    case '}':
        return l.newToken(RBRACE, string(ch))
    case ';':
        return l.newToken(SEMICOLON, string(ch))
    case ',':
        return l.newToken(COMMA, string(ch))
    }

    return Token{Type: EOF, Line: l.line, Column: l.column}
}

// peek returns the current character without advancing the position
func (l *Lexer) peek() rune {
    ch, _ := utf8.DecodeRuneInString(l.input[l.pos:])
    return ch
}

// advance moves to the next character in the input
func (l *Lexer) advance() {
    ch, size := utf8.DecodeRuneInString(l.input[l.pos:])
    l.pos += size
    if ch == '\n' {
        l.line++
        l.column = 1
    } else {
        l.column++
    }
}

// skipWhitespace skips over whitespace characters
func (l *Lexer) skipWhitespace() {
    for unicode.IsSpace(l.peek()) {
        l.advance()
    }
}

// skipComment skips over comments
func (l *Lexer) skipComment() {
    for l.peek() != '\n' && l.pos < len(l.input) {
        l.advance()
    }
}

// readIdentifier reads an identifier or keyword
func (l *Lexer) readIdentifier() Token {
    start := l.pos
    for unicode.IsLetter(l.peek()) || unicode.IsDigit(l.peek()) || l.peek() == '_' {
        l.advance()
    }
    literal := l.input[start:l.pos]
    return Token{Type: l.lookupIdentifier(literal), Literal: literal, Line: l.line, Column: l.column}
}

// readNumber reads a number
func (l *Lexer) readNumber() Token {
    start := l.pos
    for unicode.IsDigit(l.peek()) {
        l.advance()
    }
    literal := l.input[start:l.pos]
    return Token{Type: NUMBER, Literal: literal, Line: l.line, Column: l.column}
}

// newToken creates a new token of the given type and literal value
func (l *Lexer) newToken(tokenType TokenType, literal string) Token {
    token := Token{Type: tokenType, Literal: literal, Line: l.line, Column: l.column}
    l.advance()
    return token
}

// lookupIdentifier determines if an identifier is a keyword or a regular identifier
func (l *Lexer) lookupIdentifier(identifier string) TokenType {
    keywords := map[string]TokenType{
        "let":     LET,
        "if":      IF,
        "elif":    ELIF,
        "else":    ELSE,
        "while":   WHILE,
        "foreach": FOREACH,
        "return":  RETURN,
        "int":     INT,
        "string":  STRING,
        "bool":    BOOL,
        "void":    VOID,
    }
    if tokenType, ok := keywords[identifier]; ok {
        return tokenType
    }
    return IDENTIFIER
}