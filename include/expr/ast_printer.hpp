#include "expr.hpp"


namespace lox {
    struct Ast_Printer{
        auto print(Expr& expr) -> std::string;
        auto operator()(Expr_Monostate) -> std::string;
        auto operator()(Box<Binary>& expr) -> std::string;
        auto operator()(Box<Grouping>& expr) -> std::string;
        auto operator()(Box<Literal>& expr) -> std::string;
        auto operator()(Box<Unary>& expr) -> std::string;
        auto operator()(Box<Variable>& expr) -> std::string;
        auto operator()(Box<Assign>& expr) -> std::string;
        auto operator()(Box<Logical>& expr) -> std::string;
        private:
        template<typename... T>
        auto parenthisize(std::string name, T... exprs) -> std::string;
    };
};