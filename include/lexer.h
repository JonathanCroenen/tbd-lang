#pragma once

#include <string>
#include <unordered_map>

#include "token.h"

class Lexer {
public:
    Lexer(const std::string& input);

    Token NextToken();

private:
    void Advance();
    char Peek();

    Token CreateToken(Token::Type type, std::string literal);

    void SkipWhitespace();
    Token ReadIdentifier();
    Token ReadNumber();

    std::string _input;
    uint _position;
    uint _read_position;
    char _char;

    uint _line;
    uint _column;
};

inline std::unordered_map<std::string, Token::Type> keywords = {
    {"fn", Token::Type::FUNCTION},
    {"let", Token::Type::LET},
    {"if", Token::Type::IF},
    {"else", Token::Type::ELSE},
    {"return", Token::Type::RETURN},
    {"true", Token::Type::TRUE},
    {"false", Token::Type::FALSE},
    {"or", Token::Type::OR},
    {"and", Token::Type::AND},
};

