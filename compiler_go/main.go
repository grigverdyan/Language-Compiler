package main

import (
    "fmt"
    "io/ioutil"
    "log"
    "compiler/lexer"
    "compiler/parser"
    "os"
)

func main() {
    // Check if a file path is provided as an argument
    if len(os.Args) < 2 {
        log.Fatal("Please provide the path to the source file.")
    }

    // Read the file content
    filePath := os.Args[1]
    source, err := ioutil.ReadFile(filePath)
    if err != nil {
        log.Fatalf("Failed to read file: %v", err)
    }

    // Create a new lexer and tokenize the source code
    lex := lexer.New(string(source))
    tokens := []lexer.Token{}
    for tok := lex.NextToken(); tok.Type != lexer.EOF; tok = lex.NextToken() {
        tokens = append(tokens, tok)
    }

    // Create a new parser and parse the tokens into an AST
    p := parser.New(tokens)
	program, err := p.Parse()
    if err != nil {
        log.Fatalf("Failed to parse tokens: %v", err)
    }

    // Print the AST
    fmt.Println("AST:")
    fmt.Printf("%+v\n", program)
}