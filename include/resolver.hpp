#ifndef LOX_RESOLVER_HPP
#define LOX_RESOLVER_HPP
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "box.hpp"
#include "expr.hpp"
#include "interpreter.hpp"
#include "stmt.hpp"
#include "token.hpp"

namespace lox {
class Resolver {
public:
  Resolver(Interpreter &t_interpreter);

  auto begin_scope() -> void;
  auto end_scope() -> void;
  auto resolve(Expr const &expr) -> void;
  auto resolve(Stmt const &stmt) -> void;
  auto resolve(std::vector<Stmt> const &statements) -> void;

  auto declare(Token const &name) -> void;
  auto define(Token const &name) -> void;

  auto operator()(Expr_Monostate /*unused*/) -> void;
  auto operator()(Stmt_Monostate /*unused*/) -> void;
  auto operator()(Box<Binary> const &expr) -> void;
  auto operator()(Box<Grouping> const &expr) -> void;
  auto operator()(Box<Literal> const &expr) -> void;
  auto operator()(Box<Unary> const &expr) -> void;
  auto operator()(Box<Print> const &stmt) -> void;
  auto operator()(Box<Expression> const &stmt) -> void;
  auto operator()(Box<Variable> const &expr) -> void;
  auto operator()(Box<Var> const &Stmt) -> void;
  auto operator()(Box<Assign> const &expr) -> void;
  auto operator()(Box<Block> const &stmt) -> void;
  auto operator()(Box<If> const &stmt) -> void;
  auto operator()(Box<Logical> const &expr) -> void;
  auto operator()(Box<While> const &stmt) -> void;
  auto operator()(Box<Call> const &expr) -> void;
  auto operator()(Box<Function> const &stmt) -> void;
  auto operator()(Box<Return> const &stmt) -> void;

private:
  enum class Function_Type { NONE, FUNCTION };

  std::reference_wrapper<Interpreter> m_interpreter;
  std::vector<std::unordered_map<std::string, bool>> m_scopes;

  auto resolve_local(Expr const &expr, Token const &name) -> void;
  auto resolve_function(Box<Function> const &function, Function_Type type)
      -> void;

  Function_Type current_function{};
};
}; // namespace lox
#endif