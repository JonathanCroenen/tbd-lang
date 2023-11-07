#include "object.h"


std::ostream& operator<<(std::ostream& stream, const Object& object) {
    object.Print(stream);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, Object::Type type) {
    switch (type) {
        case Object::Type::INT:
            stream << "INT";
            break;
        case Object::Type::BOOL:
            stream << "BOOL";
            break;
        case Object::Type::NIL:
            stream << "NIL";
            break;
        case Object::Type::RETURN:
            stream << "RETURN";
            break;
        case Object::Type::FUNCTION:
            stream << "FUNCTION";
            break;
        case Object::Type::ERROR:
            stream << "ERROR";
            break;
    }

    return stream;
}

void Integer::Print(std::ostream& stream) const {
    stream << value;
}

void Boolean::Print(std::ostream& stream) const {
    stream << (value ? "true" : "false");
}

void Nil::Print(std::ostream& stream) const {
    stream << "nil";
}

void Return::Print(std::ostream& stream) const {
    stream << "return " << *value;
}

void Function::Print(std::ostream& stream) const {
    stream << "fn(";
    for (size_t i = 0; i < parameters.size(); i++) {
        stream << *parameters[i];
        if (i != parameters.size() - 1) {
            stream << ", ";
        }
    }
    stream << ") " << *body;
}

void Error::Print(std::ostream& stream) const {
    stream << "error: " << message;
}
