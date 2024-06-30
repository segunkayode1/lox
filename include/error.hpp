#ifndef LOX_ERROR_HPP
#define LOX_ERROR_HPP

#include <stdexcept>
#include <string>

#include "token.hpp"

namespace lox {
extern bool had_error;         // NOLINT : Interpreter requires global value
extern bool had_runtime_error; // NOLINT : Interpreter requires global value

auto report(int line, std::string const &where, std::string const &message)
    -> void;
auto error(int line, std::string const &message) -> void;
auto error(Token token, std::string const &message) -> void;

class Runtime_Error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  Runtime_Error(Token t_token, std::string t_message);
  Token m_token;
};

auto runtime_error(Runtime_Error const &err) -> void;
}; // namespace lox

#endif