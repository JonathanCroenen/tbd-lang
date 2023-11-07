#include "lexer.h"



inline bool IsAlphaNumerical(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

inline bool IsDigit(char c) {
    return '0' <= c && c <= '9';
}

inline bool IsWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}


Lexer::Lexer(const std::string& input)
    : _input(input), _position(0), _read_position(1), _char(input[0]) {}

Token Lexer::NextToken() {
    while (IsWhitespace(_char)) {
        Advance();
    }

    Token token = Token(Token::Type::ILLEGAL, "");

    switch (_char) {
    case '+':
        token = Token(Token::Type::PLUS, "+");
        break;
    case '-':
        token = Token(Token::Type::MINUS, "-");
        break;
    case '*':
        token = Token(Token::Type::ASTERISK, "*");
        break;
    case '/':
        token = Token(Token::Type::SLASH, "/");
        break;
    case '=':
        if (Peek() == '=') {
            Advance();
            token = Token(Token::Type::EQUAL, "==");
        } else {
            token = Token(Token::Type::ASSIGN, "=");
        }
        break;
    case '!':
        if (Peek() == '=') {
            Advance();
            token = Token(Token::Type::NOT_EQUAL, "!=");
        } else {
        token = Token(Token::Type::BANG, "!");
            }
        break;
    case '<':
        token = Token(Token::Type::LESS_THAN, "<");
        break;
    case '>':
        token = Token(Token::Type::GREATER_THAN, ">");
        break;
    case ',':
        token = Token(Token::Type::COMMA, ",");
        break;
    case ';':
        token = Token(Token::Type::SEMICOLON, ";");
        break;
    case '(':
        token = Token(Token::Type::LPAREN, "(");
        break;
    case ')':
        token = Token(Token::Type::RPAREN, ")");
        break;
    case '{':
        token = Token(Token::Type::LBRACE, "{");
        break;
    case '}':
        token = Token(Token::Type::RBRACE, "}");
        break;
    case '\0':
        token = Token(Token::Type::EOF, "");
        break;
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
        return ReadIdentifier();
    case '0' ... '9':
        return ReadNumber();
    }

    Advance();
    return token;
}

void Lexer::Advance() {
    if (_read_position >= _input.length()) {
        _char = '\0';
    } else {
        _char = _input[_read_position];
    }

    _position = _read_position;
    _read_position++;
}

char Lexer::Peek() {
    if (_read_position >= _input.length()) {
        return '\0';
    } else {
        return _input[_read_position];
    }
}

Token Lexer::ReadIdentifier() {
    uint position = _position;
    while (IsAlphaNumerical(_char) || IsDigit(_char)) {
        Advance();
    }

    std::string literal = _input.substr(position, _position - position);

    if (keywords.find(literal) != keywords.end()) {
        return Token(keywords[literal], literal);
    }

    return Token(Token::Type::IDENT, literal);
}

Token Lexer::ReadNumber() {
    uint position = _position;
    while (IsDigit(_char)) {
        Advance();
    }
    return Token(Token::Type::INT, _input.substr(position, _position - position));
}
