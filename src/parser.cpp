#include "parser.hpp"
#include "expr/expr.hpp"
#include "token_type.hpp"
#include "error.hpp"

namespace lox{
    Parser::Parser(std::vector<Token> t_tokens)
    :m_tokens{t_tokens}
    ,m_current{} {};

    
    auto Parser::match(auto... types) -> bool{
        for(auto type: {types...}){
            if(check(type)){
                advance();
                return true;
            }
        }
        return false;
    }

    auto Parser::advance() -> Token{
        if(!is_at_end()) m_current++;
        return previous();
    }

    auto Parser::check(Token_Type type) -> bool{
        return peek().get_token_type() == type;
    }

    auto Parser::previous() -> Token{
        return m_tokens[m_current - 1];
    }

    auto Parser::peek() -> Token{
        return m_tokens[m_current];
    }

    auto Parser::is_at_end() -> bool{
        return peek().get_token_type() == Token_Type::END_OF_FILE;
    }
    auto Parser::expression() -> Expr{
        return equality();
    }

    auto Parser::consume(Token_Type type, std::string const& err_message) -> Token{
        if(check(type)){
           return advance();
        }
        throw error(peek(), err_message);
    }

    auto Parser::equality() -> Expr{
        using enum Token_Type;
        auto expr = comparison();
        while(match(BANG_EQUAL, EQUAL_EQUAL)){
            auto op = previous();
            auto right = comparison();
            expr = Box<Binary>{expr, op, right};
        }
        return expr;
    }

    auto Parser::comparison() -> Expr{
        using enum Token_Type;
        auto expr = term();
        while(match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)){
            auto op = previous();
            auto right = term();
            expr = Box<Binary>{expr, op, right};
        }
        return expr;
    }

    auto Parser::term() -> Expr{
        using enum Token_Type;
        auto expr = factor();
        while(match(PLUS, MINUS)){
            auto op = previous();
            auto right = factor();
            expr = Box<Binary>{expr, op, right};
        }
        return expr;
    }

    auto Parser::factor() -> Expr{
        using enum Token_Type;
        auto expr = unary();
        while(match(STAR, SLASH)){
            auto op = previous();
            auto right = unary();
            expr = Box<Binary>{expr, op, right};
        }
        return expr;
    }

    auto Parser::unary() -> Expr{
        using enum Token_Type;
        if(match(MINUS, BANG)){
            auto op = previous();
            auto right = unary();
            return Box<Unary>{op, right};
        }

        return primary();
    }
    auto Parser::primary() -> Expr{
        using enum Token_Type;
        if(match(NUMBER)){
            return Box<Literal>{previous().get_literal()};
        }

        if(match(STRING)){
            return Box<Literal>{previous().get_literal()};
        }

        if(match(TRUE)){
            return Box<Literal>{true};
        }

        if(match(FALSE)){
            return Box<Literal>{false};
        }
        if(match(NIL)){
            return Expr{};
        }

        if(match(LEFT_PAREN)){
            Expr expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression");
            return Box<Grouping>(expr);
        }

        throw error(peek(), "Expected expression");
    }

    auto Parser::error(Token token, std::string const& message) -> Parser_Error{
        lox::error(token, message);
        return Parser_Error{message};
    }

    auto Parser::parse() -> Expr{
        try{
            return expression();
        }catch(Parser_Error error){
            return Expr{};
        }
    }

    
};