#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H
#include <vector>

#include "expr.hpp"
#include "stmt.hpp"
#include "enviroment.hpp"

namespace lox{
    struct Interpreter{
        Interpreter();
        auto interpret(std::vector<Stmt> statements) -> void;
        auto evaluate(Expr expr) -> Object;
        auto execute(Stmt stmt) -> void;
        auto operator()(Expr_Monostate /*unused*/) -> Object;
        auto operator()(Stmt_Monostate /*unused*/) -> void;
        auto operator()(Box<Binary>& expr) -> Object;
        auto operator()(Box<Grouping>& expr) -> Object;
        auto operator()(Box<Literal>& expr) -> Object;
        auto operator()(Box<Unary>& expr) -> Object;
        auto operator()(Box<Print>& stmt) -> void;
        auto operator()(Box<Expression>& stmt) -> void;
        auto operator()(Box<Variable>& expr) -> Object;
        auto operator()(Box<Var>& Stmt) -> void;
        auto operator()(Box<Assign>& expr) -> Object;
        auto operator()(Box<Block>& stmt) -> void;
        private:
        std::unique_ptr<Enviroment> enviroment;
    };
};

#endif