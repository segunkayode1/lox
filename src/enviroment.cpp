#include <memory>
#include <string>
#include <utility>

#include "enviroment.hpp"
#include "error.hpp"
#include "object.hpp"
#include "token.hpp"

namespace lox {
Enviroment::Enviroment() = default;

Enviroment::Enviroment(std::shared_ptr<Enviroment> t_parent)
    : m_parent{std::move(t_parent)} {}

auto Enviroment::define(std::string const &name, Object const &value) -> void {
  m_values[name] = value;
}

auto Enviroment::assign(Token const &name, Object const &value) -> void {
  if (m_values.contains(name.get_lexume())) {
    m_values[name.get_lexume()] = value;
    return;
  }

  if (m_parent) {
    m_parent->assign(name, value);
    return;
  }

  throw Runtime_Error{name,
                      "Undefined Indentifier : '" + name.get_lexume() + "'."};
}

auto Enviroment::assign_at(int distance, Token const &name, Object const &value)
    -> void {
  if (distance <= 0 and m_values.contains(name.get_lexume())) {
    m_values[name.get_lexume()] = value;
    return;
  }

  if (distance > 0 and m_parent) {
    m_parent->assign_at(distance - 1, name, value);
    return;
  }

  throw Runtime_Error{name,
                      "Undefined Indentifier : '" + name.get_lexume() + "'."};
}

auto Enviroment::get(Token const &name) -> Object {
  if (m_values.contains(name.get_lexume())) {
    return m_values[name.get_lexume()];
  }

  if (m_parent) {
    return m_parent->get(name);
  }

  throw Runtime_Error{name,
                      "Undefined Indentifier : '" + name.get_lexume() + "'."};
}

auto Enviroment::get_parent() -> std::shared_ptr<Enviroment> & {
  return m_parent;
}

auto Enviroment::get_at(int distance, Token const &name) -> Object {
  if (distance <= 0 and m_values.contains(name.get_lexume())) {
    return m_values[name.get_lexume()];
  }

  if (distance > 0 and m_parent) {
    return m_parent->get_at(distance - 1, name);
  }

  throw Runtime_Error{name,
                      "Undefined Indentifier : '" + name.get_lexume() + "'."};
}
} // namespace lox