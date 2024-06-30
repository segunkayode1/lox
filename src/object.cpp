#include <any>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "box.hpp"
#include "interpreter.hpp"
#include "object.hpp"
#include "stmt.hpp"

namespace lox {
Func::Func(
    std::function<auto()->int> t_arity,
    std::function<auto(Interpreter &, std::vector<Object> &)->Object> t_call,
    std::string t_name)
    : arity{std::move(t_arity)}, call{std::move(t_call)},
      name{std::move(t_name)} {}

Lox_Function::Lox_Function(std::any t_declaration,
                           std::shared_ptr<Enviroment> &t_closure)
    : m_declaration{std::move(t_declaration)}, m_closure{t_closure} {}

auto Lox_Function::arity() -> std::size_t {
  auto declaration = std::any_cast<Box<Function>>(m_declaration);
  return declaration->m_params.size();
}

auto Lox_Function::call(Interpreter &interpreter,
                        std::vector<Object> &arguments) -> Object {
  auto declaration = std::any_cast<Box<Function>>(m_declaration);
  auto enviroment = std::make_shared<Enviroment>(m_closure);
  for (int i = 0; i < declaration->m_params.size(); i++) {
    enviroment->define(declaration->m_params[i].get_lexume(), arguments[i]);
  }

  try {
    interpreter.execute_block(declaration->m_body, enviroment);
  } catch (Object const &obj) {
    return obj;
  }

  return {};
}
}; // namespace lox