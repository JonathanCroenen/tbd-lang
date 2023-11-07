#include "evaluator.h"
#include <iostream>

std::shared_ptr TRUE = std::make_shared<Boolean>(true);
std::shared_ptr FALSE = std::make_shared<Boolean>(true);
std::shared_ptr NIL = std::make_shared<Nil>();

bool IsTruthy(ObjectPtr object) {
    if (object == TRUE) {
        return true;
    }
    if (object == FALSE) {
        return false;
    }
    if (object == NIL) {
        return false;
    }
    if (object->type == Object::Type::INTEGER) {
        return static_cast<Integer*>(object.get())->value != 0;
    }

    return true;
}

ObjectPtr Evaluator::Evaluate(const Program& node, EnvironmentPtr environment) {
    ObjectPtr result = NIL;
    for (const Statement* statement : node.statements) {
        result = EvalStatement(statement, environment);
        if (result->type == Object::Type::RETURN) {
            return static_cast<Return*>(result.get())->value;
        }

        if (result->type == Object::Type::ERROR) {
            return result;
        }
    }

    return result;
}

ObjectPtr Evaluator::EvalStatement(Statement const* statement,
                                   EnvironmentPtr environment) {
    switch (statement->type) {
    case Statement::Type::LET:
        return EvalLet(static_cast<LetStatement const*>(statement), environment);
    case Statement::Type::RETURN:
        return EvalReturn(static_cast<ReturnStatement const*>(statement), environment);
    case Statement::Type::EXPRESSION:
        return EvalExpressionStatement(static_cast<ExpressionStatement const*>(statement),
                                       environment);
    }

    return std::make_shared<Error>("found impossible statement type");
}

ObjectPtr Evaluator::EvalLet(LetStatement const* node, EnvironmentPtr environment) {
    ObjectPtr value = EvalExpression(node->value, environment);
    if (value->type == Object::Type::ERROR) {
        return value;
    }
    if (value-> type == Object::Type::FUNCTION) {
        Function* function = static_cast<Function*>(value.get());
        function->environment->Set(node->name->value, value);
    }

    environment->Set(node->name->value, value);

    return NIL;
}

ObjectPtr Evaluator::EvalReturn(ReturnStatement const* node, EnvironmentPtr environment) {
    ObjectPtr value = EvalExpression(node->value, environment);
    return std::make_shared<Return>(value);
}

ObjectPtr Evaluator::EvalExpressionStatement(ExpressionStatement const* node,
                                             EnvironmentPtr environment) {
    return EvalExpression(node->expression, environment);
}

ObjectPtr Evaluator::EvalExpression(Expression const* node, EnvironmentPtr environment) {
    switch (node->type) {
    case Expression::Type::INT:
        return EvalIntLiteral(static_cast<IntegerLiteral const*>(node));
    case Expression::Type::BOOLEAN:
        return EvalBoolLiteral(static_cast<BooleanLiteral const*>(node));
    case Expression::Type::PREFIX:
        return EvalPrefix(static_cast<PrefixExpression const*>(node), environment);
    case Expression::Type::INFIX:
        return EvalInfix(static_cast<InfixExpression const*>(node), environment);
    case Expression::Type::IF_ELSE:
        return EvalIfElse(static_cast<IfElseExpression const*>(node), environment);
    case Expression::Type::BLOCK:
        return EvalBlock(static_cast<BlockExpression const*>(node), environment);
    case Expression::Type::IDENT:
        return EvalIdentifier(static_cast<Identifier const*>(node), environment);
    case Expression::Type::FUNCTION:
        return EvalFunction(static_cast<FunctionExpression const*>(node), environment);
    case Expression::Type::CALL:
        return EvalCall(static_cast<CallExpression const*>(node), environment);
    }

    return std::make_shared<Error>("found impossible expression type");
}

ObjectPtr Evaluator::EvalIntLiteral(IntegerLiteral const* node) {
    return std::make_shared<Integer>(node->value);
}

ObjectPtr Evaluator::EvalBoolLiteral(BooleanLiteral const* node) {
    return node->value ? TRUE : FALSE;
}

ObjectPtr Evaluator::EvalIdentifier(Identifier const* node, EnvironmentPtr environment) {
    ObjectPtr value = environment->Get(node->value);
    if (value == nullptr) {
        return std::make_shared<Error>("identifier not found: " + node->value);
    }

    return value;
}

ObjectPtr Evaluator::EvalPrefix(PrefixExpression const* node,
                                EnvironmentPtr environment) {
    ObjectPtr right = EvalExpression(node->right, environment);
    if (right->type == Object::Type::ERROR) {
        return right;
    }

    switch (node->op) {
    case PrefixExpression::Operation::Not:
        return IsTruthy(right) ? FALSE : TRUE;
    case PrefixExpression::Operation::Negate:
        return std::make_shared<Integer>(-static_cast<Integer*>(right.get())->value);
    }

    return std::make_shared<Error>("found impossible prefix operator");
}

ObjectPtr Evaluator::EvalInfix(InfixExpression const* node, EnvironmentPtr environment) {
    ObjectPtr left = EvalExpression(node->left, environment);
    if (left->type == Object::Type::ERROR) {
        return left;
    }
    ObjectPtr right = EvalExpression(node->right, environment);
    if (right->type == Object::Type::ERROR) {
        return right;
    }

    if (left->type == Object::Type::INTEGER && right->type == Object::Type::INTEGER) {
        return EvalIntInfix(static_cast<Integer*>(left.get()),
                            static_cast<Integer*>(right.get()),
                            node->op);
    }

    if (left->type == Object::Type::BOOLEAN && right->type == Object::Type::BOOLEAN) {
        return EvalBoolInfix(static_cast<Boolean*>(left.get()),
                             static_cast<Boolean*>(right.get()),
                             node->op);
    }

    // TODO print the actual types
    return std::make_shared<Error>("type mismatch");
}

ObjectPtr Evaluator::EvalIntInfix(Integer const* left,
                                  Integer const* right,
                                  InfixExpression::Operation op) {
    switch (op) {
    case InfixExpression::Operation::Add:
        return std::make_shared<Integer>(left->value + right->value);
    case InfixExpression::Operation::Subtract:
        return std::make_shared<Integer>(left->value - right->value);
    case InfixExpression::Operation::Multiply:
        return std::make_shared<Integer>(left->value * right->value);
    case InfixExpression::Operation::Divide:
        return std::make_shared<Integer>(left->value / right->value);
    case InfixExpression::Operation::Equal:
        return left->value == right->value ? TRUE : FALSE;
    case InfixExpression::Operation::NotEqual:
        return left->value != right->value ? TRUE : FALSE;
    case InfixExpression::Operation::LessThan:
        return left->value < right->value ? TRUE : FALSE;
    case InfixExpression::Operation::GreaterThan:
        return left->value > right->value ? TRUE : FALSE;
    }

    return std::make_shared<Error>("found impossible integer infix expression");
}

ObjectPtr Evaluator::EvalBoolInfix(Boolean const* left,
                                   Boolean const* right,
                                   InfixExpression::Operation op) {
    switch (op) {
    case InfixExpression::Operation::Equal:
        return left->value == right->value ? TRUE : FALSE;
    case InfixExpression::Operation::NotEqual:
        return left->value != right->value ? TRUE : FALSE;
    default:
        return std::make_shared<Error>("unsupported operator for booleans");
    }
}

ObjectPtr Evaluator::EvalBlock(BlockExpression const* node, EnvironmentPtr environment) {
    ObjectPtr result = NIL;
    for (const Statement* statement : node->statements) {
        result = EvalStatement(statement, environment);
        if (result->type == Object::Type::ERROR) {
            return result;
        }
    }

    return result;
}

ObjectPtr Evaluator::EvalIfElse(IfElseExpression const* node,
                                EnvironmentPtr environment) {
    ObjectPtr condition = EvalExpression(node->condition, environment);
    if (condition->type == Object::Type::ERROR) {
        return condition;
    }

    if (IsTruthy(condition)) {
        return EvalExpression(node->consequence, environment);
    }

    if (node->alternative != nullptr) {
        return EvalExpression(node->alternative, environment);
    }

    return NIL;
}

ObjectPtr Evaluator::EvalFunction(FunctionExpression const* node, EnvironmentPtr environment) {
    // TODO: filter only the closed over variables
    EnvironmentPtr closed = std::make_shared<Environment>(*environment);
    return std::make_shared<Function>(node->parameters, node->body, closed);
}

ObjectPtr Evaluator::EvalCall(CallExpression const* node, EnvironmentPtr environment) {
    ObjectPtr function = EvalExpression(node->function, environment);
    if (function->type == Object::Type::ERROR) {
        return function;
    }

    std::vector<ObjectPtr> arguments;
    for (Expression const* argument : node->arguments) {
        ObjectPtr evaluated = EvalExpression(argument, environment);
        if (evaluated->type == Object::Type::ERROR) {
            return evaluated;
        }

        arguments.push_back(evaluated);
    }

    if (function->type == Object::Type::FUNCTION) {
        Function* function_object = static_cast<Function*>(function.get());
        if (arguments.size() != function_object->parameters.size()) {
            return std::make_shared<Error>(
                "wrong number of arguments: expected " +
                std::to_string(function_object->parameters.size()) + ", got " +
                std::to_string(arguments.size()));
        }

        EnvironmentPtr extended = std::make_shared<Environment>(function_object->environment);
        for (size_t i = 0; i < arguments.size(); ++i) {
            extended->Set(function_object->parameters[i]->value, arguments[i]);
        }

        ObjectPtr result = EvalBlock(function_object->body, extended);
        if (result->type == Object::Type::RETURN) {
            return static_cast<Return*>(result.get())->value;
        }

        return result;
    }

    return std::make_shared<Error>("not a function");
}
