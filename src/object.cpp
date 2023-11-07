#include "object.h"


std::ostream& operator<<(std::ostream& stream, const Object& object) {
    object.Print(stream);
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
