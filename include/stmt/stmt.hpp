#ifndef LOX_STMT_HPP
#define LOX_STMT_HPP
#include "token.hpp"
#include "box.hpp"
#include "expr.hpp"

#include <memory>
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
    using Stmt = std::variant<Stmt_Monostate
                             ,Box<Expression>
                             ,Box<Print>
                             ,Box<Var>
                             ,Box<Block>
                             ,Box<If>
                             ,Box<While>>;
    struct Stmt_Monostate : public std::monostate{
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

};
#endif