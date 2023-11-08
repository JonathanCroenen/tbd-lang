#include "token.h"

Token::Token(Type type, std::string literal, uint line, uint column)
    : type(type), literal(literal), line(line), column(column) {}

std::ostream& operator<<(std::ostream& stream, const Token& token) {
    return stream << token.literal;
}

std::ostream& operator<<(std::ostream& stream, Token::Type type) {
    std::string str;
    switch (type) {
    case Token::Type::ILLEGAL:
        str = "ILLEGAL";
        break;
    case Token::Type::EOF:
        str = "EOF";
        break;
    case Token::Type::IDENT:
        str = "IDENT";
        break;
    case Token::Type::INT:
        str = "INT";
        break;
    case Token::Type::ASSIGN:
        str = "=";
        break;
    case Token::Type::PLUS:
        str = "+";
        break;
    case Token::Type::COMMA:
        str = ",";
        break;
    case Token::Type::SEMICOLON:
        str = ";";
        break;
    case Token::Type::LPAREN:
        str = "(";
        break;
    case Token::Type::RPAREN:
        str = ")";
        break;
    case Token::Type::LBRACE:
        str = "{";
        break;
    case Token::Type::RBRACE:
        str = "}";
        break;
    case Token::Type::FUNCTION:
        str = "fn";
        break;
    case Token::Type::LET:
        str = "let";
        break;
    case Token::Type::BANG:
        str = "!";
        break;
    case Token::Type::MINUS:
        str = "-";
        break;
    case Token::Type::SLASH:
        str = "/";
        break;
    case Token::Type::ASTERISK:
        str = "*";
        break;
    case Token::Type::LESS:
        str = "<";
        break;
    case Token::Type::GREATER:
        str = ">";
        break;
    case Token::Type::LESS_EQUAL:
        str = "<=";
        break;
    case Token::Type::GREATER_EQUAL:
        str = ">=";
        break;
    case Token::Type::TRUE:
        str = "true";
        break;
    case Token::Type::FALSE:
        str = "false";
        break;
    case Token::Type::IF:
        str = "if";
        break;
    case Token::Type::ELSE:
        str = "else";
        break;
    case Token::Type::RETURN:
        str = "return";
        break;
    case Token::Type::EQUAL:
        str = "==";
        break;
    case Token::Type::NOT_EQUAL:
        str = "!=";
        break;
    case Token::Type::OR:
        str = "or";
        break;
    case Token::Type::AND:
        str = "and";
        break;
    }

    return stream << str;
}
