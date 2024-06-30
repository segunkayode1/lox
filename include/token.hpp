#ifndef LOX_TOKEN_HPP
#define LOX_TOKEN_HPP

#include <string>

#include "object.hpp"
#include "token_type.hpp"

namespace lox {
class Token {
public:
  Token(Token_Type t_type, std::string t_lexume, Object t_literal, int t_line);
  Token() = default;
  [[nodiscard]] auto to_string() const -> std::string;

  [[nodiscard]] auto get_token_type() const -> Token_Type;
  [[nodiscard]] auto get_lexume() const -> std::string const &;
  [[nodiscard]] auto get_literal() const -> Object const &;
  [[nodiscard]] auto get_line() const -> int;

private:
  [[nodiscard]] auto literal_to_string() const -> std::string;

  Token_Type m_type{};
  std::string m_lexume{};
  Object m_literal{};
  int m_line{};
};
}; // namespace lox
#endif