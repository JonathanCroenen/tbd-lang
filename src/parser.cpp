#include "parser.h"

#include <iostream>
#include <sstream>

Parser::Parser(Lexer& lexer)
    : _lexer(lexer), _current_token(lexer.NextToken()), _peek_token(lexer.NextToken()) {}

Program Parser::Parse() {
    Program program;

    while (_current_token.type != Token::Type::EOF) {
        Statement* statement = ParseStatement();
        if (statement != nullptr) {
            program.statements.push_back(statement);
        }
        Advance();
    }

    return program;
}

Parser::Precedence Parser::GetPrecedence(Token::Type type) {
    switch (type) {
    case Token::Type::LPAREN:
        return Precedence::CALL;
    case Token::Type::SLASH:
    case Token::Type::ASTERISK:
        return Precedence::PRODUCT;
    case Token::Type::PLUS:
    case Token::Type::MINUS:
        return Precedence::SUM;
    case Token::Type::LESS:
    case Token::Type::GREATER:
        return Precedence::LESS_GREATER;
    case Token::Type::EQUAL:
    case Token::Type::NOT_EQUAL:
    case Token::Type::LESS_EQUAL:
    case Token::Type::GREATER_EQUAL:
        return Precedence::EQUAL;
    case Token::Type::OR:
    case Token::Type::AND:
        return Precedence::AND_OR;
    default:
        return Precedence::LOWEST;
    }
}

void Parser::Advance() {
    _current_token = std::move(_peek_token);
    _peek_token = _lexer.NextToken();
}

Statement* Parser::ParseStatement() {
    switch (_current_token.type) {
    case Token::Type::LET:
        return ParseLetStatement();
    case Token::Type::RETURN:
        if (_in_function) {
            return ParseReturnStatement();
        }

        Error("Return statements can only be used inside functions", _current_token);
        return nullptr;
    default:
        return ParseExpressionStatement();
    }
}

LetStatement* Parser::ParseLetStatement() {
    if (!PeekOrError(Token::Type::IDENT)) {
        return nullptr;
    }

    Advance();
    std::string name = _current_token.literal;

    if (!PeekOrError(Token::Type::ASSIGN)) {
        return nullptr;
    }

    Advance();
    Advance();

    Expression* expression = ParseExpression(Precedence::LOWEST);
    if (expression == nullptr) {
        return nullptr;
    }

    // let is required to end in a ;
    if (!PeekOrError(Token::Type::SEMICOLON)) {
        return nullptr;
    }

    Advance();

    return new LetStatement(new Identifier(name), expression);
}

ReturnStatement* Parser::ParseReturnStatement() {
    Advance();

    Expression* expression = ParseExpression(Precedence::LOWEST);
    if (expression == nullptr) {
        return nullptr;
    }

    // return is required to end in a ;
    if (!PeekOrError(Token::Type::SEMICOLON)) {
        return nullptr;
    }

    Advance();

    return new ReturnStatement(expression);
}

ExpressionStatement* Parser::ParseExpressionStatement() {
    Expression* expression = ParseExpression(Precedence::LOWEST);
    if (expression == nullptr) {
        return nullptr;
    }

    // end in a ; unless we ended in a block like "if { ... }"
    if (_current_token.type != Token::Type::RBRACE) {
        if (!PeekOrError(Token::Type::SEMICOLON)) {
            return nullptr;
        }

        Advance();
    }


    return new ExpressionStatement(expression);
}

Expression* Parser::ParseExpression(Precedence precedence) {
    Expression* left = nullptr;

    switch (_current_token.type) {
    case Token::Type::IDENT:
        left = ParseIdentifier();
        break;
    case Token::Type::INT:
        left = ParseIntegerLiteral();
        break;
    case Token::Type::FALSE:
        left = ParseBooleanLiteral(false);
        break;
    case Token::Type::TRUE:
        left = ParseBooleanLiteral(true);
        break;
    case Token::Type::BANG:
        left = ParsePrefixExpression(PrefixExpression::Operation::NOT);
        break;
    case Token::Type::MINUS:
        left = ParsePrefixExpression(PrefixExpression::Operation::NEGATE);
        break;
    case Token::Type::LPAREN:
        left = ParseGroupedExpression();
        break;
    case Token::Type::IF:
        left = ParseIfElseExpression();
        break;
    case Token::Type::FUNCTION:
        left = ParseFunctionLiteral();
        break;
    default:
        Error("Unexpected token \"" + _current_token.literal + "\"", _current_token);
        return nullptr;
    }

    if (left == nullptr) {
        return nullptr;
    }

    while (_peek_token.type != Token::Type::SEMICOLON &&
           precedence < GetPrecedence(_peek_token.type)) {
        Advance();
        left = ParseInfixExpression(left);
    }

    return left;
}

Identifier* Parser::ParseIdentifier() { return new Identifier(_current_token.literal); }

IntegerLiteral* Parser::ParseIntegerLiteral() {
    return new IntegerLiteral(std::stoi(_current_token.literal));
}

BooleanLiteral* Parser::ParseBooleanLiteral(bool value) {
    return new BooleanLiteral(value);
}

PrefixExpression* Parser::ParsePrefixExpression(PrefixExpression::Operation op) {
    Advance();

    Expression* right = ParseExpression(Precedence::PREFIX);
    if (right == nullptr) {
        return nullptr;
    }

    return new PrefixExpression(op, right);
}

Expression* Parser::ParseGroupedExpression() {
    Advance();

    Expression* expression = ParseExpression(Precedence::LOWEST);
    if (expression == nullptr) {
        return nullptr;
    }

    if (!PeekOrError(Token::Type::RPAREN)) {
        return nullptr;
    }

    Advance();

    return expression;
}

Expression* Parser::ParseInfixExpression(Expression* left) {
    switch (_current_token.type) {
    case Token::Type::PLUS:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::ADD);
    case Token::Type::MINUS:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::SUBTRACT);
    case Token::Type::SLASH:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::DIVIDE);
    case Token::Type::ASTERISK:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::MULTIPLY);
    case Token::Type::EQUAL:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::EQUAL);
    case Token::Type::NOT_EQUAL:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::NOT_EQUAL);
    case Token::Type::LESS:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::LESS);
    case Token::Type::LESS_EQUAL:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::LESS_EQUAL);
    case Token::Type::GREATER_EQUAL:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::GREATER_EQUAL);
    case Token::Type::GREATER:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::GREATER);
    case Token::Type::AND:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::AND);
    case Token::Type::OR:
        return ParseBasicInfixExpression(left, InfixExpression::Operation::OR);
    case Token::Type::LPAREN:
        return ParseCallExpression(left);
    default:
        Error("Unexpected token \"" + _current_token.literal + "\"", _current_token);
        return nullptr;
    }
}

InfixExpression* Parser::ParseBasicInfixExpression(Expression* left,
                                                   InfixExpression::Operation op) {
    Parser::Precedence precedence = GetPrecedence(_current_token.type);
    Advance();

    Expression* right = ParseExpression(precedence);
    if (right == nullptr) {
        return nullptr;
    }

    return new InfixExpression(op, left, right);
}

BlockExpression* Parser::ParseBlockExpression() {
    Advance();

    std::vector<Statement*> statements;
    while (_current_token.type != Token::Type::RBRACE &&
           _current_token.type != Token::Type::EOF) {

        Statement* statement = ParseStatement();
        if (statement == nullptr) {
            return nullptr;
        }

        statements.push_back(statement);
        Advance();
    }

    return new BlockExpression(statements);
}

IfElseExpression* Parser::ParseIfElseExpression() {
    Advance();

    Expression* condition = ParseExpression(Precedence::LOWEST);
    if (condition == nullptr) {
        return nullptr;
    }

    if (!PeekOrError(Token::Type::LBRACE)) {
        return nullptr;
    }

    Advance();

    BlockExpression* consequence = ParseBlockExpression();
    if (consequence == nullptr) {
        return nullptr;
    }

    Expression* alternative = nullptr;
    if (_peek_token.type == Token::Type::ELSE) {
        Advance();

        if (_peek_token.type == Token::Type::IF) {
            Advance();

            alternative = ParseIfElseExpression();
            if (alternative == nullptr) {
                return nullptr;
            }
        } else if (PeekOrError(Token::Type::LBRACE)) {
            Advance();

            alternative = ParseBlockExpression();
            if (alternative == nullptr) {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    return new IfElseExpression(condition, consequence, alternative);
}

FunctionExpression* Parser::ParseFunctionLiteral() {
    _in_function = true;

    if (!PeekOrError(Token::Type::LPAREN)) {
        return nullptr;
    }

    Advance();

    std::vector<Identifier*> parameters;
    while (_peek_token.type != Token::Type::RPAREN) {
        Advance();

        Identifier* parameter = ParseIdentifier();
        if (parameter == nullptr) {
            return nullptr;
        }

        parameters.push_back(parameter);

        if (_peek_token.type == Token::Type::COMMA) {
            Advance();
        }
    }

    Advance();

    if (!PeekOrError(Token::Type::LBRACE)) {
        return nullptr;
    }

    Advance();

    BlockExpression* body = ParseBlockExpression();
    if (body == nullptr) {
        return nullptr;
    }

    _in_function = false;
    return new FunctionExpression(parameters, body);
}

CallExpression* Parser::ParseCallExpression(Expression* left) {
    std::vector<Expression*> arguments;

    while (_peek_token.type != Token::Type::RPAREN) {
        Advance();

        Expression* argument = ParseExpression(Precedence::LOWEST);
        if (argument == nullptr) {
            return nullptr;
        }

        arguments.push_back(argument);

        if (_peek_token.type == Token::Type::COMMA) {
            Advance();
        }
    }

    Advance();

    return new CallExpression(left, arguments);
}

void Parser::Error(const std::string& message, const Token& token) {
    _errors.emplace_back(message, token.line, token.column);
}

void Parser::ExpectedError(Token::Type expected, const Token& found) {
    std::stringstream str;
    str << "Expected token \"" << expected << "\", but found \"" << found << "\"";
    Error(str.str(), found);
}

bool Parser::PeekOrError(Token::Type type) {
    if (_peek_token.type != type) {
        ExpectedError(type, _peek_token);
        return false;
    }

    return true;
}
