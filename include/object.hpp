#ifndef LOX_OBJECT_HPP
#define LOX_OBJECT_HPP

#include <any>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace lox {
class Interpreter;
class Enviroment;

struct Func;
struct Lox_Function;

using Lox_Callable = std::variant<std::monostate, Func, Lox_Function>;

using Object =
    std::variant<std::monostate, double, bool, std::string, Lox_Callable>;

struct Func {
  std::string name;
  std::function<auto()->int> arity;
  std::function<auto(Interpreter &, std::vector<Object> &)->Object> call;
  Func(std::function<auto()->int> t_arity,
       std::function<auto(Interpreter &, std::vector<Object> &)->Object> t_call,
       std::string t_name);
};

struct Lox_Function {
  Lox_Function(std::any t_declaration, std::shared_ptr<Enviroment> &t_closure);
  auto arity() -> std::size_t;
  auto call(Interpreter &interpreter, std::vector<Object> &arguments) -> Object;
  std::any m_declaration;
  std::reference_wrapper<std::shared_ptr<Enviroment>> m_closure;
};

} // namespace lox

#endif