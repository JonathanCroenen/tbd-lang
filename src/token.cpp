#include "token.h"


Token::Token(Type type, std::string literal) : type(type), literal(literal) {}

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
        str = "ASSIGN";
        break;
    case Token::Type::PLUS:
        str = "PLUS";
        break;
    case Token::Type::COMMA:
        str = "COMMA";
        break;
    case Token::Type::SEMICOLON:
        str = "SEMICOLON";
        break;
    case Token::Type::LPAREN:
        str = "LPAREN";
        break;
    case Token::Type::RPAREN:
        str = "RPAREN";
        break;
    case Token::Type::LBRACE:
        str = "LBRACE";
        break;
    case Token::Type::RBRACE:
        str = "RBRACE";
        break;
    case Token::Type::FUNCTION:
        str = "FUNCTION";
        break;
    case Token::Type::LET:
        str = "LET";
        break;
    case Token::Type::BANG:
        str = "BANG";
        break;
    case Token::Type::MINUS:
        str = "MINUS";
        break;
    case Token::Type::SLASH:
        str = "SLASH";
        break;
    case Token::Type::ASTERISK:
        str = "ASTERISK";
        break;
    case Token::Type::LESS_THAN:
        str = "LESS_THAN";
        break;
    case Token::Type::GREATER_THAN:
        str = "GREATER_THAN";
        break;
    case Token::Type::TRUE:
        str = "TRUE";
        break;
    case Token::Type::FALSE:
        str = "FALSE";
        break;
    case Token::Type::IF:
        str = "IF";
        break;
    case Token::Type::ELSE:
        str = "ELSE";
        break;
    case Token::Type::RETURN:
        str = "RETURN";
        break;
    case Token::Type::EQUAL:
        str = "EQUAL";
        break;
    case Token::Type::NOT_EQUAL:
        str = "NOT_EQUAL";
        break;
    }

    return stream << str;
}
