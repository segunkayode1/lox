#include "expr.hpp"

namespace lox{
    Binary::Binary(Expr t_left, Token t_operator, Expr t_right)
    :m_left{t_left}
    ,m_operator{t_operator}
    ,m_right{t_right} {}

    Grouping::Grouping(Expr t_expression)
    :m_expression{t_expression} {}

    Literal::Literal(std::any t_value)
    :m_value{t_value} {}

    Unary::Unary(Token t_operator, Expr t_right)
    :m_operator{t_operator}
    ,m_right{t_right} {}

};