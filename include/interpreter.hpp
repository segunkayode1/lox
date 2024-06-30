#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <vector>

#include "box.hpp"
#include "enviroment.hpp"
#include "expr.hpp"
#include "object.hpp"
#include "stmt.hpp"
#include "token.hpp"

namespace lox {
struct Interpreter {
  Interpreter();
  auto interpret(std::vector<Stmt> statements) -> void;
  auto get_enviroment() -> std::shared_ptr<Enviroment> &;
  auto get_globals() -> std::shared_ptr<Enviroment> &;
  auto execute_block(std::vector<Stmt> statements,
                     std::shared_ptr<Enviroment> t_enviroment) -> void;
  auto evaluate(Expr expr) -> Object;
  auto execute(Stmt stmt) -> void;
  auto operator()(Expr_Monostate /*unused*/) -> Object;
  auto operator()(Stmt_Monostate /*unused*/) -> void;
  auto operator()(Box<Binary> const &expr) -> Object;
  auto operator()(Box<Grouping> const &expr) -> Object;
  auto operator()(Box<Literal> const &expr) -> Object;
  auto operator()(Box<Unary> const &expr) -> Object;
  auto operator()(Box<Print> const &stmt) -> void;
  auto operator()(Box<Expression> const &stmt) -> void;
  auto operator()(Box<Variable> const &expr) -> Object;
  auto operator()(Box<Var> const &Stmt) -> void;
  auto operator()(Box<Assign> const &expr) -> Object;
  auto operator()(Box<Block> const &stmt) -> void;
  auto operator()(Box<If> const &stmt) -> void;
  auto operator()(Box<Logical> const &expr) -> Object;
  auto operator()(Box<While> const &stmt) -> void;
  auto operator()(Box<Call> const &expr) -> Object;
  auto operator()(Box<Function> const &stmt) -> void;
  auto operator()(Box<Return> const &stmt) -> void;
  auto resolve(Expr const &expr, int depth) -> void;

private:
  std::shared_ptr<Enviroment> enviroment;
  std::shared_ptr<Enviroment> globals;
  std::unordered_map<std::size_t, int> locals;

  auto look_up_variable(Token const &name, Expr const &expr) -> Object;
};
}; // namespace lox

#endif