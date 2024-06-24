#ifndef LOX_PARSER_HPP
#define LOX_PARSER_HPP

#include <vector>

#include "expr/expr.hpp"

namespace lox{
    class Parser{
        public:
        Parser(std::vector<Token> t_tokens);
        auto parse() -> Expr;
        private:
        std::vector<Token> m_tokens;
        int m_current;
        auto expression() -> Expr;
        auto equality() -> Expr;
        auto comparison() -> Expr;
        auto term() -> Expr;
        auto factor() -> Expr;
        auto unary() -> Expr;
        auto primary() -> Expr;


        auto match(auto ...types) -> bool;
        auto advance() -> Token;
        auto check(Token_Type type) -> bool;
        auto previous() -> Token;
        auto peek() -> Token;
        auto is_at_end() -> bool;
        auto consume(Token_Type type, std::string const& err_message) -> Token;
        
        class Parser_Error : public std::runtime_error{
            public:
            using std::runtime_error::runtime_error;
        };

        auto error(Token token, std::string const& message) -> Parser_Error;
    };
};

#endif