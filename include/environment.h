#pragma once

#include "object.h"
#include <memory>
#include <unordered_map>
#include <ostream>

struct Object;

struct Environment {
    Environment() = default;
    Environment(const Environment&) = default;
    Environment(std::shared_ptr<Environment> outer) : outer(outer) {}

    std::shared_ptr<Object> Get(const std::string& name) const;
    void Set(const std::string& name, std::shared_ptr<Object> value);
    void Remove(const std::string& name);

    std::shared_ptr<Environment> outer = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Object>> store;

    friend std::ostream& operator<<(std::ostream& stream, const Environment& environment);
};

std::ostream& operator<<(std::ostream& stream, const Environment& environment);
