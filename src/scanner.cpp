#include <cctype>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "error.hpp"
#include "object.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "token_type.hpp"

namespace lox {
Scanner::Scanner(std::string t_source) : m_source{std::move(t_source)} {};

auto Scanner::is_at_end() const -> bool { return m_current >= m_source.size(); }

auto Scanner::advance() -> char { return m_source[m_current++]; }

auto Scanner::peek() -> char {
  return is_at_end() ? '\0' : m_source[m_current];
}

auto Scanner::peek_next() -> char {
  return m_current + 1 >= m_source.size() ? '\0' : m_source[m_current + 1];
}

auto Scanner::add_token(Token_Type t_type) -> void {
  add_token(t_type, Object{});
}

auto Scanner::add_token(Token_Type t_type, Object t_literal) -> void {
  auto text = m_source.substr(m_start, m_current - m_start);
  m_tokens.emplace_back(t_type, text, t_literal, m_line);
}

auto Scanner::match(char rune) -> bool {
  if (is_at_end() or peek() != rune) {
    return false;
  }
  (void)advance();
  return true;
}

auto isalpha(char rune) -> bool {
  return static_cast<bool>(std::isalpha(rune)) or rune == '_';
}

auto isalphnum(char rune) -> bool {
  return static_cast<bool>(std::isdigit(rune)) or isalpha(rune);
}

auto Scanner::scan_token() -> void {
  using enum Token_Type;
  auto rune = advance();
  switch (rune) {
  case '(':
    add_token(LEFT_PAREN);
    break;
  case ')':
    add_token(RIGHT_PAREN);
    break;
  case '{':
    add_token(LEFT_BRACE);
    break;
  case '}':
    add_token(RIGHT_BRACE);
    break;
  case ',':
    add_token(COMMA);
    break;
  case '.':
    add_token(DOT);
    break;
  case '-':
    add_token(MINUS);
    break;
  case '+':
    add_token(PLUS);
    break;
  case ';':
    add_token(SEMICOLON);
    break;
  case '*':
    add_token(STAR);
    break;
  case '!':
    add_token(match('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    add_token(match('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '>':
    add_token(match('=') ? GREATER_EQUAL : GREATER);
    break;
  case '<':
    add_token(match('=') ? LESS_EQUAL : LESS);
    break;
  case '/':
    if (match('/')) {
      while (not is_at_end() and peek() != '\n') {
        advance();
      }
    } else {
      add_token(SLASH);
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    m_line++;
    break;
  case '"':
    string();
    break;

  default:
    if (static_cast<bool>(std::isdigit(rune))) {
      number();
      break;
    }

    if (isalpha(rune)) {
      indentifier();
      break;
    }

    lox::error(m_line, "Unexpected character");
    break;
  }
}

auto Scanner::scan_tokens() -> std::vector<Token> {
  while (!is_at_end()) {
    m_start = m_current;
    scan_token();
  }
  m_tokens.emplace_back(Token_Type::END_OF_FILE, "", Object{}, m_line);
  return m_tokens;
}

auto Scanner::string() -> void {
  auto value = std::string();

  for (; not is_at_end() and peek() != '"'; advance()) {
    if (peek() == '\n') {
      m_line++;
    }
    value.push_back(peek());
  }
  if (is_at_end()) {
    lox::error(m_line, "Unterminated string.");
    return;
  }

  // consume last "
  advance();

  add_token(Token_Type::STRING, Object{value});
}

auto Scanner::number() -> void {
  while (static_cast<bool>(std::isdigit(peek()))) {
    advance();
  }
  if (peek() == '.' && static_cast<bool>(std::isdigit(peek_next()))) {
    advance();
    while (static_cast<bool>(std::isdigit(peek()))) {
      advance();
    }
  }

  auto text = m_source.substr(m_start, m_current - m_start);
  add_token(Token_Type::NUMBER, std::stod(text));
}
namespace {
auto build_map() -> std::unordered_map<std::string, Token_Type> {
  using enum Token_Type;
  return std::unordered_map<std::string, Token_Type>{
      {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
      {"fun", FUN},   {"for", FOR},     {"if", IF},         {"nil", NIL},
      {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
      {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE}};
}
}; // namespace

const std::unordered_map<std::string, Token_Type> Scanner::keywords =
    build_map();

auto Scanner::indentifier() -> void {
  while (isalphnum(peek())) {
    advance();
  }

  auto text = m_source.substr(m_start, m_current - m_start);
  if (keywords.contains(text)) {
    add_token(keywords.at(text));
    return;
  }

  add_token(Token_Type::IDENTIFIER);
}
}; // namespace lox