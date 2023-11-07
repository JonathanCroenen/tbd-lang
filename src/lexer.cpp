#include "lexer.h"

inline bool IsAlphaNumerical(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

inline bool IsDigit(char c) { return '0' <= c && c <= '9'; }

inline bool IsWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

Lexer::Lexer(const std::string& input)
    : _input(input), _position(0), _read_position(1), _char(input[0]), _line(0),
      _column(0) {}

Token Lexer::NextToken() {
    SkipWhitespace();

    Token token = CreateToken(Token::Type::ILLEGAL, "");

    switch (_char) {
    case '+':
        token = CreateToken(Token::Type::PLUS, "+");
        break;
    case '-':
        token = CreateToken(Token::Type::MINUS, "-");
        break;
    case '*':
        token = CreateToken(Token::Type::ASTERISK, "*");
        break;
    case '/':
        token = CreateToken(Token::Type::SLASH, "/");
        break;
    case '=':
        if (Peek() == '=') {
            Advance();
            token = CreateToken(Token::Type::EQUAL, "==");
        } else {
            token = CreateToken(Token::Type::ASSIGN, "=");
        }
        break;
    case '!':
        if (Peek() == '=') {
            Advance();
            token = CreateToken(Token::Type::NOT_EQUAL, "!=");
        } else {
            token = CreateToken(Token::Type::BANG, "!");
        }
        break;
    case '<':
        if (Peek() == '=') {
            Advance();
            token = CreateToken(Token::Type::LESS_EQUAL, "<=");
        } else {
            token = CreateToken(Token::Type::LESS, "<");
        }
        break;
    case '>':
        if (Peek() == '=') {
            Advance();
            token = CreateToken(Token::Type::GREATER_EQUAL, ">=");
        } else {
            token = CreateToken(Token::Type::GREATER, ">");
        }
        break;
    case ',':
        token = CreateToken(Token::Type::COMMA, ",");
        break;
    case ';':
        token = CreateToken(Token::Type::SEMICOLON, ";");
        break;
    case '(':
        token = CreateToken(Token::Type::LPAREN, "(");
        break;
    case ')':
        token = CreateToken(Token::Type::RPAREN, ")");
        break;
    case '{':
        token = CreateToken(Token::Type::LBRACE, "{");
        break;
    case '}':
        token = CreateToken(Token::Type::RBRACE, "}");
        break;
    case '\0':
        token = CreateToken(Token::Type::EOF, "");
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
    _column++;
}

char Lexer::Peek() {
    if (_read_position >= _input.length()) {
        return '\0';
    } else {
        return _input[_read_position];
    }
}

Token Lexer::CreateToken(Token::Type type, std::string literal) {
    return Token(type, literal, _line, _column);
}

void Lexer::SkipWhitespace() {
    while (IsWhitespace(_char)) {
        if (_char == '\n') {
            _line++;
            _column = 0;
        }
        Advance();
    }
}

Token Lexer::ReadIdentifier() {
    uint position = _position;
    while (IsAlphaNumerical(_char) || IsDigit(_char)) {
        Advance();
    }

    std::string literal = _input.substr(position, _position - position);

    if (keywords.find(literal) != keywords.end()) {
        return CreateToken(keywords[literal], literal);
    }

    return CreateToken(Token::Type::IDENT, literal);
}

Token Lexer::ReadNumber() {
    uint position = _position;
    while (IsDigit(_char)) {
        Advance();
    }
    return CreateToken(Token::Type::INT, _input.substr(position, _position - position));
}
