#pragma once

#include <string>
#include <vector>
#include <exception>

#include "lexer.h"
#include "ast.h"

// TODO: reference token or line number or something
class ParseError : public std::exception {
public:
    ParseError(const std::string& message) : _message(message) {}

    const char* what() const noexcept override {
        return _message.c_str();
    }

private:
    std::string _message;
};

class Parser {
public:
    Parser(Lexer& lexer);

    Program Parse();
    std::vector<ParseError> GetErrors() const { return _errors; }

private:
    enum Precedence {
        LOWEST,
        EQUAL,
        LESS_GREATER,
        SUM,
        PRODUCT,
        PREFIX,
        CALL,
    };

    Precedence GetPrecedence(Token::Type type);

    Lexer& _lexer;
    Token _current_token;
    Token _peek_token;

    std::vector<ParseError> _errors;

    void Advance();
    Statement* ParseStatement();
    LetStatement* ParseLetStatement();
    ReturnStatement* ParseReturnStatement();
    ExpressionStatement* ParseExpressionStatement();
    Expression* ParseExpression(Precedence precedence);
    Identifier* ParseIdentifier();
    IntegerLiteral* ParseIntegerLiteral();
    BooleanLiteral* ParseBooleanLiteral(bool value);
    PrefixExpression* ParsePrefixExpression(PrefixExpression::Operation op);
    Expression* ParseInfixExpression(Expression* left);
    InfixExpression* ParseBasicInfixExpression(Expression* left,
                                               InfixExpression::Operation op);
    CallExpression* ParseCallExpression(Expression* left);
    Expression* ParseGroupedExpression();
    BlockExpression* ParseBlockExpression();
    IfElseExpression* ParseIfElseExpression();
    FunctionExpression* ParseFunctionLiteral();

    void Error(const std::string& message);
    bool PeekOrError(Token::Type type);
};
