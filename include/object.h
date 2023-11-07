#pragma once

#include "ast.h"
#include "environment.h"

#include <memory>

struct Environment;

struct Object {
    enum Type {
        INTEGER,
        BOOLEAN,
        NIL,
        RETURN,
        FUNCTION,
        ERROR,
    };

    Type type;

    friend std::ostream& operator<<(std::ostream& stream, const Object& object);

protected:
    Object(Type type) : type(type) {}

    virtual void Print(std::ostream& stream) const = 0;
};

struct Integer : Object {
    Integer(int value) : Object(Type::INTEGER), value(value) {}

    int value;

protected:
    virtual void Print(std::ostream& stream) const override;
};

struct Boolean : Object {
    Boolean(bool value) : Object(Type::BOOLEAN), value(value) {}

    bool value;

protected:
    virtual void Print(std::ostream& stream) const override;
};

struct Nil : Object {
    Nil() : Object(Type::NIL) {}

protected:
    virtual void Print(std::ostream& stream) const override;
};

struct Return : Object {
    Return(std::shared_ptr<Object> value) : Object(Type::RETURN), value(value) {}

    std::shared_ptr<Object> value;

protected:
    virtual void Print(std::ostream& stream) const override;
};

struct Function : Object {
    Function(std::vector<Identifier*> parameters,
             BlockExpression* body,
             std::shared_ptr<Environment> environment)
        : Object(Type::FUNCTION), parameters(parameters), body(body),
          environment(std::move(environment)) {}

    std::vector<Identifier*> parameters;
    BlockExpression* body;
    std::shared_ptr<Environment> environment;

protected:
    virtual void Print(std::ostream& stream) const override;
};

struct Error : Object {
    Error(std::string message) : Object(Type::ERROR), message(message) {}

    std::string message;

protected:
    virtual void Print(std::ostream& stream) const override;
};
