#pragma once

#include <string>
#undef EOF
#include <vector>
#include <ostream>


struct Statement;
struct Identifier;
struct Expression;

// Program
struct Program {
    std::vector<Statement*> statements;

    friend std::ostream& operator<<(std::ostream& stream, const Program& program);
};

// Statements
struct Statement {
    enum Type {
        LET,
        RETURN,
        EXPRESSION,
    };

    friend std::ostream& operator<<(std::ostream& stream, const Statement& statement);

    Type type;

protected:
    Statement(Type type) : type(type) {}

    virtual void Print(std::ostream& stream) const = 0;
};

// let <IDENT> = <EXPRESSION>;
struct LetStatement : Statement {
public:
    LetStatement(Identifier* name, Expression* value) : Statement(Type::LET), name(name), value(value) {}

    Identifier* name;
    Expression* value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// return <EXPRESSION>;
struct ReturnStatement : Statement {
    ReturnStatement(Expression* value) : Statement(Type::RETURN), value(value) {}

    Expression* value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <EXPRESSION>;
struct ExpressionStatement : Statement {
    ExpressionStatement(Expression* expression) : Statement(Type::EXPRESSION), expression(expression) {}

    Expression* expression;

private:
    virtual void Print(std::ostream& stream) const override;
};

// Expressions
struct Expression {
    enum Type {
        IDENT,
        INT,
        BOOLEAN,
        PREFIX,
        INFIX,
        BLOCK,
        IF_ELSE,
        CALL,
        FUNCTION,
    };

    friend std::ostream& operator<<(std::ostream& stream, const Expression& expression);

    Type type;

protected:
    Expression(Type type) : type(type) {}

    virtual void Print(std::ostream& stream) const = 0;
};

// <IDENT>
struct Identifier : Expression {
    Identifier(std::string value) : Expression(Type::IDENT), value(value) {}

    std::string value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <0-9_>*
struct IntegerLiteral : Expression {
    IntegerLiteral(int value) : Expression(Type::INT), value(value) {}

    int value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// true | false
struct BooleanLiteral : Expression {
    BooleanLiteral(bool value) : Expression(Type::BOOLEAN), value(value) {}

    bool value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <OPERATOR><EXPRESSION>
struct PrefixExpression : Expression {
    enum Operation {
        NEGATE,
        NOT,
    };

    PrefixExpression(Operation op, Expression* right) : Expression(Type::PREFIX), op(op), right(right) {}

    friend std::ostream& operator<<(std::ostream& stream, Operation op);

    Operation op;
    Expression* right;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <EXPRESSION><OPERATOR><EXPRESSION>
struct InfixExpression : Expression {
    enum Operation {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        LESS,
        GREATER,
        LESS_EQUAL,
        GREATER_EQUAL,
        EQUAL,
        NOT_EQUAL,
    };

    InfixExpression(Operation op, Expression* left, Expression* right)
        : Expression(Type::INFIX), op(op), left(left), right(right) {}

    friend std::ostream& operator<<(std::ostream& stream, Operation op);

    Operation op;
    Expression* left;
    Expression* right;

private:
    virtual void Print(std::ostream& stream) const override;
};

// { <STATEMENT>* }
struct BlockExpression : Expression {
    BlockExpression(std::vector<Statement*> statements) : Expression(Type::BLOCK), statements(statements) {}

    std::vector<Statement*> statements;

private:
    virtual void Print(std::ostream& stream) const override;
};

// if (<EXPRESSION>) <BLOCK> else <BLOCK> | <IF_ELSE_EXPRESSION>
struct IfElseExpression : Expression {
    IfElseExpression(Expression* condition, BlockExpression* consequence,
                     Expression* alternative)
        : Expression(Type::IF_ELSE), condition(condition), consequence(consequence), alternative(alternative) {}

    Expression* condition;
    BlockExpression* consequence;
    Expression* alternative;

private:
    virtual void Print(std::ostream& stream) const override;
};

// fn(<IDENT>,*) <BLOCK>
struct FunctionExpression : Expression {
    FunctionExpression(std::vector<Identifier*> parameters, BlockExpression* body)
        : Expression(Type::FUNCTION), parameters(parameters), body(body) {}

    std::vector<Identifier*> parameters;
    BlockExpression* body;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <EXPRESSION>(<EXPRESSION>,*)
struct CallExpression : Expression {
    CallExpression(Expression* function, std::vector<Expression*> arguments)
        : Expression(Type::CALL), function(function), arguments(arguments) {}

    Expression* function;
    std::vector<Expression*> arguments;

private:
    virtual void Print(std::ostream& stream) const override;
};
