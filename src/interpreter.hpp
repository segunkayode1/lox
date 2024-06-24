#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include "expr.hpp"

namespace lox{
    struct Interpreter{
        auto interpret(Expr expr) -> void;
        auto evaluate(Expr expr) -> Object;
        auto operator()(std::monostate) -> Object;
        auto operator()(Box<Binary>& expr) -> Object;
        auto operator()(Box<Grouping>& expr) -> Object;
        auto operator()(Box<Literal>& expr) -> Object;
        auto operator()(Box<Unary>& expr) -> Object;
    };
};

#endif