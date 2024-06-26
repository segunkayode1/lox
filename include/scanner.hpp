#ifndef LOX_SCANNER_HPP
#define LOX_SCANNER_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "object.hpp"
#include "token.hpp"
#include "token_type.hpp"

namespace lox {
class Scanner {
public:
  Scanner(std::string t_source);

  auto scan_tokens() -> std::vector<Token>;

private:
  static const std::unordered_map<std::string, Token_Type> keywords;
  std::string m_source;
  std::vector<Token> m_tokens;

  int m_start{};
  int m_current{};
  int m_line{1};

  [[nodiscard]] auto is_at_end() const -> bool;
  auto scan_token() -> void;

  auto advance() -> char;
  auto peek() -> char;
  auto peek_next() -> char;

  auto add_token(Token_Type t_type) -> void;
  auto add_token(Token_Type t_type, Object t_literal) -> void;

  auto match(char rune) -> bool;

  auto string() -> void;
  auto number() -> void;
  auto indentifier() -> void;
};
}; // namespace lox
#endif