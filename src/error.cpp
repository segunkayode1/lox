#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "error.hpp"
#include "token.hpp"
#include "token_type.hpp"

namespace lox {
bool had_error = false;         // NOLINT : Interpreter requires global value
bool had_runtime_error = false; // NOLINT : Interpreter requires global value

auto report(int line, std::string const &where, std::string const &message)
    -> void {
  std::cerr << "[line " << line << "] Error" << where << ": " << message
            << '\n';
  had_error = true;
}

auto error(int line, std::string const &message) -> void {
  report(line, "", message);
}

auto error(Token token, std::string const &message) -> void {
  if (token.get_token_type() == Token_Type::END_OF_FILE) {
    report(token.get_line(), " at end ", message);
  } else {
    report(token.get_line(), " at '" + token.get_lexume() + "'", message);
  }
}

Runtime_Error::Runtime_Error(Token t_token, std::string t_message)
    : std::runtime_error(std::move(t_message)), m_token{std::move(t_token)} {}

auto runtime_error(Runtime_Error const &err) -> void {
  std::cout << err.what() << ": (" << err.m_token.get_lexume() << ") \n[line"
            << err.m_token.get_line() << "] \n";
  had_runtime_error = true;
}

}; // namespace lox