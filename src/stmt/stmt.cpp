#include <utility>
#include <vector>

#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"

namespace lox {
Expression::Expression(Expr t_expression)
    : m_expression{std::move(t_expression)} {}

Print::Print(Expr t_expression) : m_expression{std::move(t_expression)} {}

Var::Var(Token t_name, Expr t_initializer)
    : m_name{std::move(t_name)}, m_initializer{std::move(t_initializer)} {}

Block::Block(std::vector<Stmt> t_statements)
    : m_statements{std::move(t_statements)} {}

If::If(Expr t_condition, Stmt t_thenBranch, Stmt t_elseBranch)
    : m_condition{std::move(t_condition)},
      m_thenBranch{std::move(t_thenBranch)},
      m_elseBranch{std::move(t_elseBranch)} {}

While::While(Expr t_condition, Stmt t_body)
    : m_condition{std::move(t_condition)}, m_body{std::move(t_body)} {}

Function::Function(Token t_name, std::vector<Token> t_params,
                   std::vector<Stmt> t_body)
    : m_name{std::move(t_name)}, m_params{std::move(t_params)},
      m_body{std::move(t_body)} {}

Return::Return(Token t_keyword, Expr t_expression)
    : m_keyword{std::move(t_keyword)}, m_expression{std::move(t_expression)} {}

}; // namespace lox