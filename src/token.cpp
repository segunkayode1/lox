#include <sstream>
#include <string>
#include <utility>

#include "object.hpp"
#include "token.hpp"
#include "token_type.hpp"

namespace lox {
Token::Token(Token_Type t_type, std::string t_lexume, Object t_literal,
             int t_line)
    : m_type{t_type}, m_lexume{std::move(t_lexume)},
      m_literal{std::move(t_literal)}, m_line{t_line} {};

auto Token::literal_to_string() const -> std::string {
  switch (m_type) {
  case Token_Type::STRING:
    return std::get<std::string>(m_literal);
  case Token_Type::NUMBER:
    return std::to_string(std::get<double>(m_literal));
  default:
    return "";
  }
}

auto Token::to_string() const -> std::string {
  auto strstrm = std::stringstream{};
  strstrm << " ,Lexume: " << m_lexume;
  strstrm << " ,Literal: " << literal_to_string();
  strstrm << " ,Line Number: " << m_line << '\n';
  return strstrm.str();
}

auto Token::get_token_type() const -> Token_Type { return m_type; }
auto Token::get_lexume() const -> std::string const & { return m_lexume; }
auto Token::get_literal() const -> Object const & { return m_literal; }
auto Token::get_line() const -> int { return m_line; }
} // namespace lox