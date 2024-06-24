#ifndef LOX_EXPR_HPP
#define LOX_EXPR_HPP
#include "token.hpp"
#include "box.hpp"

#include <memory>
#include <variant>

namespace lox {
    class Binary;
    class Grouping;
    class Literal;
    class Unary;
    using Expr = std::variant<std::monostate
                             ,Box<Binary>
                             ,Box<Grouping>
                             ,Box<Literal>
                             ,Box<Unary>>;
    struct Binary {
        Binary(Expr t_left, Token t_operator, Expr t_right);
        Expr m_left;
        Token m_operator;
        Expr m_right;
    };

    struct Grouping {
        Grouping(Expr t_expression);
        Expr m_expression;
    };

    struct Literal {
        Literal(Object t_value);
        Object m_value;
    };

    struct Unary {
        Unary(Token t_operator, Expr t_right);
        Token m_operator;
        Expr m_right;
    };

};
#endif