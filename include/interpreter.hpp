#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H
#include <vector>

#include "expr.hpp"
#include "stmt.hpp"
#include "enviroment.hpp"
#include "object.hpp"

namespace lox{
    struct Interpreter{
        Interpreter();
        auto interpret(std::vector<Stmt> statements) -> void;
        auto get_enviroment() -> std::shared_ptr<Enviroment>&;
        auto get_globals() -> std::shared_ptr<Enviroment>&;
        auto execute_block(std::vector<Stmt> statments, std::shared_ptr<Enviroment> t_enviroment) -> void;
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
        auto operator()(Box<If>& stmt) -> void;
        auto operator()(Box<Logical>& expr) -> Object;
        auto operator()(Box<While>& stmt) -> void;
        auto operator()(Box<Call>& expr) -> Object;
        auto operator()(Box<Function>& stmt) -> void;
        auto operator()(Box<Return>& stmt) -> void;
        private:
        std::shared_ptr<Enviroment> enviroment;
        std::shared_ptr<Enviroment>& globals;
    };
};

#endif