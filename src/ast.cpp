#include "ast.h"

std::ostream& operator<<(std::ostream& stream, const Statement& statement) {
    statement.Print(stream);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Expression& expression) {
    expression.Print(stream);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Program& program) {
    for (const Statement* statement : program.statements) {
        stream << *statement << "\n";
    }
    return stream;
}

void LetStatement::Print(std::ostream& stream) const {
    stream << "let " << *name << " = " << *value << ";";
}

void ReturnStatement::Print(std::ostream& stream) const {
    stream << "return " << *value << ";";
}

void ExpressionStatement::Print(std::ostream& stream) const {
    stream << *expression << ";";
}

void Identifier::Print(std::ostream& stream) const {
    stream << value;
}

void IntegerLiteral::Print(std::ostream& stream) const {
    stream << value;
}

void BooleanLiteral::Print(std::ostream& stream) const {
    stream << (value ? "true" : "false");
}

void PrefixExpression::Print(std::ostream& stream) const {
    switch (op) {
    case Operation::NEGATE:
        stream << "-";
        break;
    case Operation::NOT:
        stream << "!";
        break;
    }
    stream << *right;
}

std::ostream& operator<<(std::ostream& stream, PrefixExpression::Operation op) {
    switch (op) {
        case PrefixExpression::Operation::NEGATE:
            stream << "-";
            break;
        case PrefixExpression::Operation::NOT:
            stream << "!";
            break;
    }

    return stream;
}

void InfixExpression::Print(std::ostream& stream) const {
    stream << "(" << *left << " ";
    switch (op) {
    case Operation::ADD:
        stream << "+";
        break;
    case Operation::SUBTRACT:
        stream << "-";
        break;
    case Operation::MULTIPLY:
        stream << "*";
        break;
    case Operation::DIVIDE:
        stream << "/";
        break;
    case Operation::LESS:
        stream << "<";
        break;
    case Operation::GREATER:
        stream << ">";
        break;
    case Operation::EQUAL:
        stream << "==";
        break;
    case Operation::NOT_EQUAL:
        stream << "!=";
        break;
    case Operation::LESS_EQUAL:
        stream << "<=";
        break;
    case Operation::GREATER_EQUAL:
        stream << ">=";
        break;
    case Operation::AND:
        stream << "&&";
        break;
    case Operation::OR:
        stream << "||";
        break;
    }
    stream << " " << *right << ")";
}

std::ostream& operator<<(std::ostream& stream, InfixExpression::Operation op) {
    switch (op) {
        case InfixExpression::Operation::ADD:
            stream << "+";
            break;
        case InfixExpression::Operation::SUBTRACT:
            stream << "-";
            break;
        case InfixExpression::Operation::MULTIPLY:
            stream << "*";
            break;
        case InfixExpression::Operation::DIVIDE:
            stream << "/";
            break;
        case InfixExpression::Operation::LESS:
            stream << "<";
            break;
        case InfixExpression::Operation::GREATER:
            stream << ">";
            break;
        case InfixExpression::Operation::EQUAL:
            stream << "==";
            break;
        case InfixExpression::Operation::NOT_EQUAL:
            stream << "!=";
            break;
        case InfixExpression::Operation::LESS_EQUAL:
            stream << "<=";
            break;
        case InfixExpression::Operation::GREATER_EQUAL:
            stream << ">=";
            break;
        case InfixExpression::Operation::AND:
            stream << "and";
            break;
        case InfixExpression::Operation::OR:
            stream << "or";
            break;
    }

    return stream;
}

void BlockExpression::Print(std::ostream& stream) const {
    stream << "{ ";
    for (const Statement* statement : statements) {
        stream << *statement << " ";
    }
    stream << "}";
}

void IfElseExpression::Print(std::ostream& stream) const {
    stream << "if " << *condition << " " << *consequence;
    if (alternative != nullptr) {
        stream << " else " << *alternative;
    }
}

void FunctionExpression::Print(std::ostream& stream) const {
    stream << "fn(";
    for (size_t i = 0; i < parameters.size(); ++i) {
        stream << *parameters[i];
        if (i != parameters.size() - 1) {
            stream << ", ";
        }
    }
    stream << ") " << *body;
}

void CallExpression::Print(std::ostream& stream) const {
    stream << *function << "(";
    for (size_t i = 0; i < arguments.size(); ++i) {
        stream << *arguments[i];
        if (i != arguments.size() - 1) {
            stream << ", ";
        }
    }
    stream << ")";
}
