#include <any>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "box.hpp"
#include "box_id.hpp"
#include "enviroment.hpp"
#include "error.hpp"
#include "expr.hpp"
#include "interpreter.hpp"
#include "lox_callable.hpp"
#include "object.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "token_type.hpp"

namespace lox {

auto to_string_object(Object obj) -> std::string {
  if (std::holds_alternative<bool>(obj)) {
    return std::get<bool>(obj) ? "true" : "false";
  }
  if (std::holds_alternative<std::string>(obj)) {
    return std::get<std::string>(obj);
  }

  if (std::holds_alternative<double>(obj)) {
    auto dbl = std::get<double>(obj);
    if (dbl == static_cast<int>(dbl)) {
      return std::to_string(static_cast<int>(dbl));
    }
    return std::to_string(dbl);
  }

  if (std::holds_alternative<Lox_Callable>(obj)) {
    auto function = std::get<Lox_Callable>(obj);
    if (std::holds_alternative<Lox_Function>(function)) {
      auto lox_function = std::get<Lox_Function>(function);
      auto declaration =
          std::any_cast<Box<Function>>(lox_function.m_declaration);
      return "<Fn " + declaration->m_name.get_lexume() + ">";
    }

    if (std::holds_alternative<Func>(function)) {
      auto func = std::get<Func>(function);
      return "<Fn " + func.name + ">";
    }
  }

  return "nil";
}

auto Interpreter::interpret(std::vector<Stmt> statements) -> void {
  try {
    for (auto statement : statements) {
      execute(statement);
    }
  } catch (Runtime_Error const &err) {
    runtime_error(err);
  }
}

auto Interpreter::execute(Stmt stmt) -> void { std::visit(*this, stmt); }

auto Interpreter::evaluate(Expr expr) -> Object {
  return std::visit(*this, expr);
}
auto Interpreter::operator()(Expr_Monostate /*unused*/) -> Object { return {}; }

auto Interpreter::operator()(Stmt_Monostate /*unused*/) -> void {}

auto is_equal(Object const &left, Object const &right) {
  auto const both_numbers = std::holds_alternative<double>(left) and
                            std::holds_alternative<double>(right);
  auto const both_strings = std::holds_alternative<std::string>(left) and
                            std::holds_alternative<std::string>(right);
  auto const both_bools = std::holds_alternative<bool>(left) and
                          std::holds_alternative<bool>(right);
  auto const both_nil = std::holds_alternative<std::monostate>(left) and
                        std::holds_alternative<std::monostate>(right);
  if (both_numbers) {
    return std::get<double>(left) == std::get<double>(right);
  }
  if (both_strings) {
    return std::get<std::string>(left) == std::get<std::string>(right);
  }
  if (both_bools) {
    return std::get<bool>(left) == std::get<bool>(right);
  }
  return both_nil;
}

auto Interpreter::operator()(Box<Binary> const &expr) -> Object {
  auto const left = evaluate(expr->m_left);
  auto const right = evaluate(expr->m_right);

  using enum Token_Type;

  auto both_numbers = false;
  auto both_strings = false;

  switch (expr->m_operator.get_token_type()) {
  case MINUS:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);

    if (both_numbers) {
      return std::get<double>(left) - std::get<double>(right);
    }

    throw Runtime_Error{expr->m_operator, "operands have to be numbers"};

  case PLUS:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);

    both_strings = std::holds_alternative<std::string>(left) and
                   std::holds_alternative<std::string>(right);

    if (both_numbers) {
      return std::get<double>(left) + std::get<double>(right);
    }

    if (both_strings) {
      return std::get<std::string>(left) + std::get<std::string>(right);
    }

    throw Runtime_Error{expr->m_operator,
                        "operands have to be numbers or strings"};

  case EQUAL_EQUAL:
    return is_equal(left, right);

  case BANG_EQUAL:
    return not is_equal(left, right);

  case GREATER:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);

    if (both_numbers) {
      return std::get<double>(left) > std::get<double>(right);
    }

    throw Runtime_Error{expr->m_operator, "operands have to be numbers"};

  case GREATER_EQUAL:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);

    if (both_numbers) {
      return std::get<double>(left) >= std::get<double>(right);
    }

    throw Runtime_Error{expr->m_operator, "operands have to be numbers"};

  case LESS:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);

    if (both_numbers) {
      return std::get<double>(left) < std::get<double>(right);
    }

    throw Runtime_Error{expr->m_operator, "operands have to be numbers"};

  case LESS_EQUAL:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);

    if (both_numbers) {
      return std::get<double>(left) <= std::get<double>(right);
    }

    throw Runtime_Error{expr->m_operator, "operands have to be numbers"};

  case STAR:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);

    if (both_numbers) {
      return std::get<double>(left) * std::get<double>(right);
    }

    throw Runtime_Error{expr->m_operator, "operands have to be numbers"};
  case SLASH:
    both_numbers = std::holds_alternative<double>(left) and
                   std::holds_alternative<double>(right);
    if (both_numbers) {
      return std::get<double>(left) / std::get<double>(right);
    }

    throw Runtime_Error{expr->m_operator, "operands have to be numbers"};

  default:
    return {};
  }
  return {};
}

auto Interpreter::operator()(Box<Grouping> const &expr) -> Object {
  return evaluate(expr->m_expression);
}

auto Interpreter::operator()(Box<Literal> const &expr) -> Object {
  return expr->m_value;
}

auto truthy(Object obj) {
  if (std::holds_alternative<std::monostate>(obj)) {
    return false;
  }
  if (std::holds_alternative<bool>(obj)) {
    return std::get<bool>(obj);
  }
  return true;
}

auto Interpreter::operator()(Box<Var> const &stmt) -> void {
  auto const value = evaluate(stmt->m_initializer);
  enviroment->define(stmt->m_name.get_lexume(), value);
};

auto Interpreter::look_up_variable(Token const &name, Expr const &expr)
    -> Object {
  auto box_id = get_id(expr);
  if (locals.contains(box_id)) {
    return enviroment->get_at(locals[box_id], name);
  }
  return globals->get(name);
}

auto Interpreter::operator()(Box<Variable> const &expr) -> Object {
  return look_up_variable(expr->m_name, expr);
};

auto Interpreter::operator()(Box<Unary> const &expr) -> Object {
  auto value = evaluate(expr->m_right);
  switch (expr->m_operator.get_token_type()) {
  case Token_Type::MINUS:
    if (not std::holds_alternative<double>(value)) {
      throw Runtime_Error{expr->m_operator, "operand has to be a number"};
    }
    return std::get<double>(value) * -1;
  case Token_Type::BANG:
    return not truthy(value);
  default:
    throw Runtime_Error{expr->m_operator, "Parser Error"};
  }
}

auto Interpreter::operator()(Box<Print> const &stmt) -> void {
  auto const value = evaluate(stmt->m_expression);
  std::cout << to_string_object(value) << '\n';
}

auto Interpreter::operator()(Box<Expression> const &stmt) -> void {
  evaluate(stmt->m_expression);
}

auto Interpreter::operator()(Box<Assign> const &expr) -> Object {
  auto value = evaluate(expr->m_value);
  auto box_id = get_id(expr);
  if (locals.contains(box_id)) {
    enviroment->assign_at(locals[box_id], expr->m_name, value);
  } else {
    globals->assign(expr->m_name, value);
  }

  return value;
}

auto Interpreter::execute_block(std::vector<Stmt> statements,
                                std::shared_ptr<Enviroment> t_enviroment)
    -> void {
  auto previous = enviroment;
  enviroment = t_enviroment;
  try {
    for (auto statement : statements) {
      execute(statement);
    }
  } catch (Runtime_Error const &err) {
    enviroment = previous;
    throw err;
  } catch (Object const &obj) {
    enviroment = previous;
    throw obj;
  }
  enviroment = previous;
}

auto Interpreter::operator()(Box<Block> const &stmt) -> void {
  execute_block(stmt->m_statements, std::make_shared<Enviroment>(enviroment));
}

auto Interpreter::operator()(Box<If> const &stmt) -> void {
  if (truthy(evaluate(stmt->m_condition))) {
    execute(stmt->m_thenBranch);
  } else {
    execute(stmt->m_elseBranch);
  }
}

auto Interpreter::operator()(Box<Logical> const &expr) -> Object {
  auto left = evaluate(expr->m_left);
  if (expr->m_operator.get_token_type() == Token_Type::AND) {
    if (!truthy(left)) {
      return left;
    }
  } else {
    if (truthy(left)) {
      return left;
    }
  }
  return evaluate(expr->m_right);
}

auto Interpreter::operator()(Box<While> const &stmt) -> void {
  while (truthy(evaluate(stmt->m_condition))) {
    execute(stmt->m_body);
  }
}

auto Interpreter::operator()(Box<Call> const &expr) -> Object {
  Object callee = evaluate(expr->m_callee);

  std::vector<Object> arguments;
  for (auto const &argument : expr->m_arguments) {
    arguments.push_back(evaluate(argument));
  }

  if (not std::holds_alternative<Lox_Callable>(callee)) {
    throw Runtime_Error{expr->m_paren, "Can only call functions and classes."};
  }
  auto function = std::get<Lox_Callable>(callee);
  if (auto arity = Arity{}.arity(function); arity != arguments.size()) {
    throw Runtime_Error(expr->m_paren, "Expected " + std::to_string(arity) +
                                           " arguments but got " +
                                           std::to_string(arguments.size()));
  }
  return Caller{*this, arguments}.call(function);
}

auto Interpreter::operator()(Box<Function> const &stmt) -> void {
  auto function = Lox_Function{stmt, enviroment};
  enviroment->define(stmt->m_name.get_lexume(), function);
}

auto Interpreter::operator()(Box<Return> const &stmt) -> void {
  throw evaluate(stmt->m_expression);
}

Interpreter::Interpreter()
    : enviroment{std::make_shared<Enviroment>()}, globals{enviroment} {
  globals->define(
      "clock",
      Func{[]() { return 0; },
           [](auto const & /*interpreter*/, auto const & /*arguments*/) {
             using namespace std::chrono;
             return static_cast<double>(
                 duration_cast<seconds>(system_clock::now().time_since_epoch())
                     .count());
           },
           "clock"});
}

auto Interpreter::get_enviroment() -> std::shared_ptr<Enviroment> & {
  return enviroment;
}

auto Interpreter::get_globals() -> std::shared_ptr<Enviroment> & {
  return globals;
}

auto Interpreter::resolve(Expr const &expr, int depth) -> void {
  locals[get_id(expr)] = depth;
}

} // namespace lox