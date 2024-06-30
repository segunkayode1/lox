#include <variant>
#include <vector>

#include "box.hpp"
#include "error.hpp"
#include "expr.hpp"
#include "interpreter.hpp"
#include "resolver.hpp"
#include "stmt.hpp"
#include "token.hpp"

namespace lox {
Resolver::Resolver(Interpreter &t_interpreter) : m_interpreter{t_interpreter} {}

auto Resolver::begin_scope() -> void { m_scopes.emplace_back(); }
auto Resolver::end_scope() -> void { m_scopes.pop_back(); }

auto Resolver::resolve(Expr const &expr) -> void { std::visit(*this, expr); }

auto Resolver::resolve(Stmt const &stmt) -> void { std::visit(*this, stmt); }

auto Resolver::resolve(std::vector<Stmt> const &statements) -> void {
  for (auto const &statement : statements) {
    resolve(statement);
  }
}

auto Resolver::declare(Token const &name) -> void {
  if (m_scopes.empty()) {
    return;
  }
  auto &scope = m_scopes.back();
  if (scope.contains(name.get_lexume())) {
    error(name, "Already variable with this name in this scope.");
  }
  scope[name.get_lexume()] = false;
}

auto Resolver::define(Token const &name) -> void {
  if (m_scopes.empty()) {
    return;
  }
  m_scopes.back()[name.get_lexume()] = true;
}

auto Resolver::operator()(Expr_Monostate /*unused*/) -> void {}

auto Resolver::operator()(Stmt_Monostate /*unused*/) -> void {}

auto Resolver::operator()(Box<Binary> const &expr) -> void {
  resolve(expr->m_left);
  resolve(expr->m_right);
}

auto Resolver::operator()(Box<Grouping> const &expr) -> void {
  resolve(expr->m_expression);
}

auto Resolver::operator()(Box<Literal> const &expr) -> void {}

auto Resolver::operator()(Box<Unary> const &expr) -> void {
  resolve(expr->m_right);
}

auto Resolver::operator()(Box<Print> const &stmt) -> void {
  resolve(stmt->m_expression);
}

auto Resolver::operator()(Box<Expression> const &stmt) -> void {
  resolve(stmt->m_expression);
}

auto Resolver::resolve_local(Expr const &expr, Token const &name) -> void {
  for (auto i = static_cast<int>(m_scopes.size()) - 1; i >= 0; i--) {
    if (m_scopes[i].contains(name.get_lexume())) {
      m_interpreter.get().resolve(expr,
                                  static_cast<int>(m_scopes.size()) - 1 - i);
      return;
    }
  }
}

auto Resolver::operator()(Box<Variable> const &expr) -> void {
  if (not m_scopes.empty() and
      (m_scopes.back().contains(expr->m_name.get_lexume()) and
       not m_scopes.back()[expr->m_name.get_lexume()])) {
    error(expr->m_name, "Can't read local variable in its own initializer.");
  }

  resolve_local(expr, expr->m_name);
}

auto Resolver::operator()(Box<Var> const &stmt) -> void {
  declare(stmt->m_name);
  resolve(stmt->m_initializer);
  define(stmt->m_name);
}

auto Resolver::operator()(Box<Assign> const &expr) -> void {
  resolve(expr->m_value);
  resolve_local(expr, expr->m_name);
}

auto Resolver::operator()(Box<Block> const &stmt) -> void {
  begin_scope();
  resolve(stmt->m_statements);
  end_scope();
}

auto Resolver::operator()(Box<If> const &stmt) -> void {
  resolve(stmt->m_condition);
  resolve(stmt->m_thenBranch);
  resolve(stmt->m_elseBranch);
}

auto Resolver::operator()(Box<Logical> const &expr) -> void {
  resolve(expr->m_left);
  resolve(expr->m_right);
}

auto Resolver::operator()(Box<While> const &stmt) -> void {
  resolve(stmt->m_condition);
  resolve(stmt->m_body);
}

auto Resolver::operator()(Box<Call> const &expr) -> void {
  resolve(expr->m_callee);
  for (auto argument : expr->m_arguments) {
    resolve(argument);
  }
}

auto Resolver::resolve_function(Box<Function> const &function,
                                Function_Type type) -> void {
  auto enclosing_function = current_function;
  current_function = type;

  begin_scope();
  for (auto const &param : function->m_params) {
    declare(param);
    define(param);
  }
  resolve(function->m_body);
  end_scope();
  current_function = enclosing_function;
}

auto Resolver::operator()(Box<Function> const &stmt) -> void {
  declare(stmt->m_name);
  define(stmt->m_name);
  resolve_function(stmt, Function_Type::FUNCTION);
}

auto Resolver::operator()(Box<Return> const &stmt) -> void {
  if (current_function != Function_Type::FUNCTION) {
    error(stmt->m_keyword, "Can't return from to-level code.");
  }

  resolve(stmt->m_expression);
}

}; // namespace lox