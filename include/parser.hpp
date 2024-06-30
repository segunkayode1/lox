#ifndef LOX_PARSER_HPP
#define LOX_PARSER_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "token_type.hpp"

namespace lox {
class Parser {
public:
  Parser(std::vector<Token> t_tokens);
  auto parse() -> std::vector<Stmt>;

private:
  std::vector<Token> m_tokens;
  int m_current{};
  auto expression() -> Expr;
  auto assginment() -> Expr;
  auto logical_or() -> Expr;
  auto logical_and() -> Expr;
  auto equality() -> Expr;
  auto comparison() -> Expr;
  auto term() -> Expr;
  auto factor() -> Expr;
  auto unary() -> Expr;
  auto call() -> Expr;
  auto primary() -> Expr;

  auto match(auto... types) -> bool;
  auto advance() -> Token;
  auto check(Token_Type type) -> bool;
  auto previous() -> Token;
  auto peek() -> Token;
  auto is_at_end() -> bool;
  auto consume(Token_Type type, std::string const &err_message) -> Token;

  class Parser_Error : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
  };

  static auto error(Token token, std::string const &message) -> Parser_Error;

  auto statement() -> Stmt;
  auto print_statement() -> Stmt;
  auto while_statement() -> Stmt;
  auto for_statement() -> Stmt;
  auto expression_statement() -> Stmt;
  auto block_statement() -> std::vector<Stmt>;
  auto declaration() -> Stmt;
  auto var_declaration() -> Stmt;
  auto if_statement() -> Stmt;
  auto synchronize() -> void;
  auto return_statement() -> Stmt;
  auto function(std::string const &kind) -> Stmt;
};
}; // namespace lox

#endif