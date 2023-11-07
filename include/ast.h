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

    Statement(Statement&& other) = default;

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

    LetStatement(LetStatement&& other) = default;

    Identifier* name;
    Expression* value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// return <EXPRESSION>;
struct ReturnStatement : Statement {
    ReturnStatement(Expression* value) : Statement(Type::RETURN), value(value) {}

    ReturnStatement(ReturnStatement&& other) = default;

    Expression* value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <EXPRESSION>;
struct ExpressionStatement : Statement {
    ExpressionStatement(Expression* expression) : Statement(Type::EXPRESSION), expression(expression) {}

    ExpressionStatement(ExpressionStatement&& other) = default;

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

    Expression(Expression&& other) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Expression& expression);

    Type type;

protected:
    Expression(Type type) : type(type) {}

    virtual void Print(std::ostream& stream) const = 0;
};

// <IDENT>
struct Identifier : Expression {
    Identifier(std::string value) : Expression(Type::IDENT), value(value) {}

    Identifier(Identifier&& other) = default;

    std::string value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <0-9_>*
struct IntegerLiteral : Expression {
    IntegerLiteral(int value) : Expression(Type::INT), value(value) {}

    IntegerLiteral(IntegerLiteral&& other) = default;

    int value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// true | false
struct BooleanLiteral : Expression {
    BooleanLiteral(bool value) : Expression(Type::BOOLEAN), value(value) {}

    BooleanLiteral(BooleanLiteral&& other) = default;

    bool value;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <OPERATOR><EXPRESSION>
struct PrefixExpression : Expression {
    enum Operation {
        Negate,
        Not,
    };

    PrefixExpression(Operation op, Expression* right) : Expression(Type::PREFIX), op(op), right(right) {}

    PrefixExpression(PrefixExpression&& other) = default;

    Operation op;
    Expression* right;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <EXPRESSION><OPERATOR><EXPRESSION>
struct InfixExpression : Expression {
    enum Operation {
        Add,
        Subtract,
        Multiply,
        Divide,
        LessThan,
        GreaterThan,
        Equal,
        NotEqual,
    };

    InfixExpression(Operation op, Expression* left, Expression* right)
        : Expression(Type::INFIX), op(op), left(left), right(right) {}

    InfixExpression(InfixExpression&& other) = default;

    Operation op;
    Expression* left;
    Expression* right;

private:
    virtual void Print(std::ostream& stream) const override;
};

// { <STATEMENT>* }
struct BlockExpression : Expression {
    BlockExpression(std::vector<Statement*> statements) : Expression(Type::BLOCK), statements(statements) {}

    BlockExpression(BlockExpression&& other) = default;

    std::vector<Statement*> statements;

private:
    virtual void Print(std::ostream& stream) const override;
};

// if (<EXPRESSION>) <BLOCK> else <BLOCK> | <IF_ELSE_EXPRESSION>
struct IfElseExpression : Expression {
    IfElseExpression(Expression* condition, BlockExpression* consequence,
                     Expression* alternative)
        : Expression(Type::IF_ELSE), condition(condition), consequence(consequence), alternative(alternative) {}

    IfElseExpression(IfElseExpression&& other) = default;

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

    FunctionExpression(FunctionExpression&& other) = default;

    std::vector<Identifier*> parameters;
    BlockExpression* body;

private:
    virtual void Print(std::ostream& stream) const override;
};

// <EXPRESSION>(<EXPRESSION>,*)
struct CallExpression : Expression {
    CallExpression(Expression* function, std::vector<Expression*> arguments)
        : Expression(Type::CALL), function(function), arguments(arguments) {}

    CallExpression(CallExpression&& other) = default;

    Expression* function;
    std::vector<Expression*> arguments;

private:
    virtual void Print(std::ostream& stream) const override;
};
