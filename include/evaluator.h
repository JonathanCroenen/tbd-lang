#pragma once

#include "ast.h"
#include "environment.h"
#include "object.h"

#include <memory>
#include <unordered_map>

using ObjectPtr = std::shared_ptr<Object>;
using EnvironmentPtr = std::shared_ptr<Environment>;

class Evaluator {
public:
    Evaluator() = default;

    ObjectPtr Evaluate(const Program& node, EnvironmentPtr environment);

private:
    ObjectPtr EvalStatement(Statement const* node, EnvironmentPtr environment);
    ObjectPtr EvalExpression(Expression const* node, EnvironmentPtr environment);
    ObjectPtr EvalLet(LetStatement const* node, EnvironmentPtr environment);
    ObjectPtr EvalReturn(ReturnStatement const* node, EnvironmentPtr environment);
    ObjectPtr EvalExpressionStatement(ExpressionStatement const* node,
                                      EnvironmentPtr environment);
    ObjectPtr EvalIntLiteral(IntegerLiteral const* node);
    ObjectPtr EvalBoolLiteral(BooleanLiteral const* node);
    ObjectPtr EvalIdentifier(Identifier const* node, EnvironmentPtr environment);
    ObjectPtr EvalPrefix(PrefixExpression const* node, EnvironmentPtr environment);
    ObjectPtr EvalInfix(InfixExpression const* node, EnvironmentPtr environment);
    ObjectPtr
    EvalIntInfix(ObjectPtr left, ObjectPtr right, InfixExpression::Operation op);
    ObjectPtr
    EvalBoolInfix(ObjectPtr left, ObjectPtr right, InfixExpression::Operation op);
    ObjectPtr EvalBlock(BlockExpression const* node, EnvironmentPtr environment);
    ObjectPtr EvalIfElse(IfElseExpression const* node, EnvironmentPtr environment);
    ObjectPtr EvalFunction(FunctionExpression const* node, EnvironmentPtr environment);
    ObjectPtr EvalCall(CallExpression const* node, EnvironmentPtr environment);
};
