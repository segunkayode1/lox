#include "stmt.hpp"

namespace lox{
    Expression::Expression(Expr t_expression)
    :m_expression{t_expression} {}

    Print::Print(Expr t_expression)
    :m_expression{t_expression} {}

    Var::Var(Token t_name, Expr t_initializer)
    :m_name{t_name}
    ,m_initializer{t_initializer} {}

    Block::Block(std::vector<Stmt> t_statements)
    :m_statements{t_statements} {}

};