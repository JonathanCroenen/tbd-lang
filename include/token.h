#pragma once

#include <string>
#undef EOF

struct Token {
    enum Type {
        // Special
        ILLEGAL,
        EOF,

        IDENT,
        // Literals
        INT,
        TRUE,
        FALSE,
        // Operators
        ASSIGN,
        PLUS,
        MINUS,
        BANG,
        ASTERISK,
        SLASH,
        LESS_THAN,
        GREATER_THAN,
        EQUAL,
        NOT_EQUAL,
        // Delimiters
        COMMA,
        SEMICOLON,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        // Keywords
        FUNCTION,
        LET,
        IF,
        ELSE,
        RETURN,
    };

    Token(Type type, std::string literal);

    Token(Token&& other) = default;
    Token& operator=(Token&& other) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Token& token);
    friend std::ostream& operator<<(std::ostream& stream, Token::Type type);

    Type type;
    std::string literal;
};
