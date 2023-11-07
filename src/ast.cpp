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
    case Operation::Negate:
        stream << "-";
        break;
    case Operation::Not:
        stream << "!";
        break;
    }
    stream << *right;
}

std::ostream& operator<<(std::ostream& stream, PrefixExpression::Operation op) {
    switch (op) {
        case PrefixExpression::Operation::Negate:
            stream << "-";
            break;
        case PrefixExpression::Operation::Not:
            stream << "!";
            break;
    }

    return stream;
}

void InfixExpression::Print(std::ostream& stream) const {
    stream << "(" << *left << " ";
    switch (op) {
    case Operation::Add:
        stream << "+";
        break;
    case Operation::Subtract:
        stream << "-";
        break;
    case Operation::Multiply:
        stream << "*";
        break;
    case Operation::Divide:
        stream << "/";
        break;
    case Operation::LessThan:
        stream << "<";
        break;
    case Operation::GreaterThan:
        stream << ">";
        break;
    case Operation::Equal:
        stream << "==";
        break;
    case Operation::NotEqual:
        stream << "!=";
        break;
    }
    stream << " " << *right << ")";
}

std::ostream& operator<<(std::ostream& stream, InfixExpression::Operation op) {
    switch (op) {
        case InfixExpression::Operation::Add:
            stream << "+";
            break;
        case InfixExpression::Operation::Subtract:
            stream << "-";
            break;
        case InfixExpression::Operation::Multiply:
            stream << "*";
            break;
        case InfixExpression::Operation::Divide:
            stream << "/";
            break;
        case InfixExpression::Operation::LessThan:
            stream << "<";
            break;
        case InfixExpression::Operation::GreaterThan:
            stream << ">";
            break;
        case InfixExpression::Operation::Equal:
            stream << "==";
            break;
        case InfixExpression::Operation::NotEqual:
            stream << "!=";
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
