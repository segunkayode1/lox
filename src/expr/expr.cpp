#include <utility>
#include <vector>

#include "expr.hpp"
#include "object.hpp"
#include "token.hpp"

namespace lox {
Binary::Binary(Expr t_left, Token t_operator, Expr t_right)
    : m_left{std::move(t_left)}, m_operator{std::move(t_operator)},
      m_right{std::move(t_right)} {}

Grouping::Grouping(Expr t_expression) : m_expression{std::move(t_expression)} {}

Literal::Literal(Object t_value) : m_value{std::move(t_value)} {}

Unary::Unary(Token t_operator, Expr t_right)
    : m_operator{std::move(t_operator)}, m_right{std::move(t_right)} {}

Variable::Variable(Token t_name) : m_name{std::move(t_name)} {}

Assign::Assign(Token t_name, Expr t_value)
    : m_name{std::move(t_name)}, m_value{std::move(t_value)} {}

Logical::Logical(Expr t_left, Token t_operator, Expr t_right)
    : m_left{std::move(t_left)}, m_operator{std::move(t_operator)},
      m_right{std::move(t_right)} {}

Call::Call(Expr t_callee, Token t_paren, std::vector<Expr> t_arguments)
    : m_callee{std::move(t_callee)}, m_paren{std::move(t_paren)},
      m_arguments{std::move(t_arguments)} {}

}; // namespace lox