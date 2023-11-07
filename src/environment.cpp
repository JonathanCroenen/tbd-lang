#include "environment.h"


std::shared_ptr<Object> Environment::Get(const std::string& name) const {
    auto it = store.find(name);
    if (it != store.end()) {
        return it->second;
    }

    if (outer != nullptr) {
        return outer->Get(name);
    }

    return nullptr;
}

void Environment::Set(const std::string& name, std::shared_ptr<Object> value) {
    store[name] = value;
}

void Environment::Remove(const std::string& name) {
    store.erase(name);
}

std::ostream& operator<<(std::ostream& stream, const Environment& environment) {
    stream << "Environment(";
    for (auto it = environment.store.begin(); it != environment.store.end(); it++) {
        stream << it->first << ": " << *it->second;
        if (std::next(it) != environment.store.end()) {
            stream << ", ";
        }
    }
    stream << ")";
    return stream;
}
