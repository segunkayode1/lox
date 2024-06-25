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

    If::If(Expr t_condition, Stmt t_thenBranch, Stmt t_elseBranch)
    :m_condition{t_condition}
    ,m_thenBranch{t_thenBranch}
    ,m_elseBranch{t_elseBranch} {}

    While::While(Expr t_condition, Stmt t_body)
    :m_condition{t_condition}
    ,m_body{t_body} {}

};