#ifndef LOX_ENV_HPP
#define LOX_ENV_HPP
#include <memory>
#include <string>
#include <unordered_map>

#include "object.hpp"
#include "token.hpp"
namespace lox {
class Enviroment {
public:
  Enviroment();
  Enviroment(std::shared_ptr<Enviroment> t_parent);
  auto define(std::string const &name, Object const &value) -> void;
  auto assign(Token const &name, Object const &value) -> void;
  auto get(Token const &name) -> Object;
  auto get_parent() -> std::shared_ptr<Enviroment> &;
  auto get_at(int distance, Token const &name) -> Object;
  auto assign_at(int distance, Token const &name, Object const &value) -> void;

private:
  std::unordered_map<std::string, Object> m_values;
  std::shared_ptr<Enviroment> m_parent;
};

}; // namespace lox

#endif