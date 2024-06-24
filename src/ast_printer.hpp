#include "expr.hpp"


namespace lox {
    struct Ast_Printer{
        auto print(Expr& expr) -> std::string;
        auto operator()(std::monostate) -> std::string;
        auto operator()(Box<Binary>& expr) -> std::string;
        auto operator()(Box<Grouping>& expr) -> std::string;
        auto operator()(Box<Literal>& expr) -> std::string;
        auto operator()(Box<Unary>& expr) -> std::string;
        private:
        template<typename... T>
        auto parenthisize(std::string name, T... exprs) -> std::string;
    };
};