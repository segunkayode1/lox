#ifndef LOX_STMT_HPP
#define LOX_STMT_HPP
#include "box.hpp"
#include "expr.hpp"
#include "token.hpp"

#include <variant>
#include <vector>

namespace lox {
struct Stmt_Monostate;
struct Expression;
struct Print;
struct Var;
struct Block;
struct If;
struct While;
struct Function;
struct Return;
using Stmt =
    std::variant<Stmt_Monostate, Box<Expression>, Box<Print>, Box<Var>,
                 Box<Block>, Box<If>, Box<While>, Box<Function>, Box<Return>>;
struct Stmt_Monostate : public std::monostate {
  using std::monostate::monostate;
};

struct Expression {
  Expression(Expr t_expression);
  Expr m_expression;
};

struct Print {
  Print(Expr t_expression);
  Expr m_expression;
};

struct Var {
  Var(Token t_name, Expr t_initializer);
  Token m_name;
  Expr m_initializer;
};

struct Block {
  Block(std::vector<Stmt> t_statements);
  std::vector<Stmt> m_statements;
};

struct If {
  If(Expr t_condition, Stmt t_thenBranch, Stmt t_elseBranch);
  Expr m_condition;
  Stmt m_thenBranch;
  Stmt m_elseBranch;
};

struct While {
  While(Expr t_condition, Stmt t_body);
  Expr m_condition;
  Stmt m_body;
};

struct Function {
  Function(Token t_name, std::vector<Token> t_params, std::vector<Stmt> t_body);
  Token m_name;
  std::vector<Token> m_params;
  std::vector<Stmt> m_body;
};

struct Return {
  Return(Token t_keyword, Expr t_expression);
  Token m_keyword;
  Expr m_expression;
};

}; // namespace lox
#endif