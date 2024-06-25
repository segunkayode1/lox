#ifndef LOX_EXPR_HPP
#define LOX_EXPR_HPP
#include "token.hpp"
#include "box.hpp"

#include <memory>
#include <variant>

namespace lox {
    struct Expr_Monostate;
    struct Binary;
    struct Grouping;
    struct Literal;
    struct Unary;
    struct Variable;
    struct Assign;
    using Expr = std::variant<Expr_Monostate
                             ,Box<Binary>
                             ,Box<Grouping>
                             ,Box<Literal>
                             ,Box<Unary>
                             ,Box<Variable>
                             ,Box<Assign>>;
    struct Expr_Monostate : public std::monostate{
        using std::monostate::monostate;
    };

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

    struct Variable {
        Variable(Token t_name);
        Token m_name;
    };

    struct Assign {
        Assign(Token t_name, Expr t_value);
        Token m_name;
        Expr m_value;
    };

};
#endif