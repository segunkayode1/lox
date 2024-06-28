#include "parser.hpp"
#include "expr.hpp"
#include "token_type.hpp"
#include "error.hpp"
#include "stmt.hpp"

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
        return assginment();
    }

    

    auto Parser::assginment() -> Expr{
        auto expr  = logical_or();

        if(match(Token_Type::EQUAL)){
            auto equals = previous();
            auto value = assginment();
            if(std::holds_alternative<Box<Variable>>(expr)){
                auto& var = std::get<Box<Variable>>(expr);
                return Box<Assign>{var->m_name, value};
            }

            error(equals, "Invalid assigment target.");
        }

        return expr;
    }

    auto Parser::logical_or() -> Expr{
        auto expr = logical_and();
        while(match(Token_Type::OR)){
            auto op = previous();
            auto right = logical_and();
            expr = Box<Logical>{expr, op, right};
        }
        return expr;
    }
    auto Parser::logical_and() -> Expr{
        auto expr = equality();
        while(match(Token_Type::AND)){
            auto op = previous();
            auto right = equality();
            expr = Box<Logical>{expr, op, right};
        }
        return expr;
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

        return call();
    }

    auto Parser::call() -> Expr{
        auto expr = primary();
        if(match(Token_Type::LEFT_PAREN)){
            if(match(Token_Type::RIGHT_PAREN)){
                return Box<Call>{expr,Token{}, std::vector<Expr>{}};
            }

            auto expressions = std::vector<Expr>{expression()};

            while(match(Token_Type::COMMA)){
                if(expressions.size() >= 255){
                    error(peek(), "Can't have more than 255 arguments");
                }
                expressions.push_back(expression());
            }

            auto token = consume(Token_Type::RIGHT_PAREN, "Expected ')' after function arguments");

            return Box<Call>{expr, token, expressions};
        }
        return expr;
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
            return Box<Literal>{Object{}};
        }

        if(match(LEFT_PAREN)){
            Expr expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression");
            return Box<Grouping>(expr);
        }

        if(match(IDENTIFIER)){
            return Box<Variable>(previous());
        }

        throw error(peek(), "Expected expression");
    }

    auto Parser::error(Token token, std::string const& message) -> Parser_Error{
        lox::error(token, message);
        return Parser_Error{message};
    }

    auto Parser::statement() -> Stmt {
        if(match(Token_Type::FOR)) return for_statement();
        if(match(Token_Type::IF)) return if_statement();
        if(match(Token_Type::PRINT)) return print_statement();
        if(match(Token_Type::RETURN)) return return_statement();
        if(match(Token_Type::WHILE)) return while_statement();
        if(match(Token_Type::LEFT_BRACE))return Box<Block>{block_statement()};

        return expression_statement();
    }

    auto Parser::expression_statement() -> Stmt{
        auto expr = expression();
        consume(Token_Type::SEMICOLON, "Expected ';' after expression" );
        return Box<Expression>{expr};
    }
  
    auto Parser::print_statement() -> Stmt{
        auto expr = expression();
        consume(Token_Type::SEMICOLON, "Expected ';' after value");
        return Box<Print>{expr};
    }

    auto Parser::var_declaration() -> Stmt{
        Token name = consume(Token_Type::IDENTIFIER, "Expected variable name.");

        Expr Initialiser{};

        if(match(Token_Type::EQUAL)){
            Initialiser = expression();
        }

        consume(Token_Type::SEMICOLON, "Expected '; after variable declaration");

        return Box<Var>{name, Initialiser};
    }

    auto Parser::declaration() -> Stmt{
        try{
            if(match(Token_Type::VAR)){
                return var_declaration();
            }
            if(match(Token_Type::FUN)){
                return function("function");
            }
            return statement();
        }catch(Parser_Error err){
            synchronize();
            return Stmt{};
        }
    }

    auto Parser::synchronize() -> void{
        advance();

        using enum Token_Type;
        while(not is_at_end()){
            if(previous().get_token_type() == SEMICOLON) return;

            switch(peek().get_token_type()){
                case CLASS:
                case FUN:
                case VAR:
                case FOR:
                case IF:
                case WHILE:
                case PRINT:
                case RETURN:
                return;
                default:
                break;
            }

            advance();
        }
    }

    auto Parser::block_statement() -> std::vector<Stmt>{
        auto statements = std::vector<Stmt>{};
        while(not is_at_end() and not check(Token_Type::RIGHT_BRACE)){
            statements.push_back(declaration());
        }
        consume(Token_Type::RIGHT_BRACE, "Expected '}' after block");
        return statements;
    }

    auto Parser::if_statement() -> Stmt{
        consume(Token_Type::LEFT_PAREN, "Expected '(' after if");
        auto const expr = expression();
        consume(Token_Type::RIGHT_PAREN, "Expected ')' after if condition");
        auto const then_branch = statement();
        auto else_branch = Stmt{};
        if(match(Token_Type::ELSE)){
            else_branch = statement();
        }

        return Box<If>{expr, then_branch, else_branch};
    }

    auto Parser::while_statement() -> Stmt{
         consume(Token_Type::LEFT_PAREN, "Expected '(' after while");
         auto const expr = expression();
         consume(Token_Type::RIGHT_PAREN, "Expected ')' after while condition");
         auto const body = statement();
         return Box<While>{expr, body};
    }

    auto Parser::for_statement() -> Stmt{
        using enum Token_Type;
        consume(Token_Type::LEFT_PAREN, "Expected '(' after while");
        auto intitialiser = Stmt{};
        if(match(VAR)){
            intitialiser = var_declaration();
        }else if(!match(SEMICOLON)){
            intitialiser = expression_statement();
        }
        auto condition = Expr{};
        if(!check(SEMICOLON)){
            condition = expression();
        }

        consume(SEMICOLON, "Expected ';' after loop condition");

        auto increment = Expr{};
        if(!check(RIGHT_PAREN)){
            increment = expression();
        }


        consume(RIGHT_PAREN, "Expected ')' after  for clauses");
        Stmt body = statement();
        if(not std::holds_alternative<Expr_Monostate>(increment)){
            std::vector<Stmt> statements;
            statements.push_back(body);
            statements.push_back(Box<Expression>(increment));
            body = Box<Block>{
                statements
            };
        }

        if(std::holds_alternative<Expr_Monostate>(increment)){
            condition = Box<Literal>{true};
        }

        body = Box<While>{condition, body};

        if(not std::holds_alternative<Stmt_Monostate>(intitialiser)){
            std::vector<Stmt> statements;
            statements.push_back(intitialiser);
            statements.push_back(body);
            body = Box<Block>{
                statements
            };
        }

        return body;
    }

    auto Parser::return_statement() -> Stmt{
        auto token = previous();
        if(match(Token_Type::SEMICOLON)){
            return Box<Return>{token, Expr{}};
        }

        auto expr = expression();

        consume(Token_Type::SEMICOLON, "Expected ';' after return");

        return Box<Return>{token, expr};
    }

    auto Parser::function(std::string const& kind) -> Stmt{
        auto  name = consume(Token_Type::IDENTIFIER, "Expected " + kind + " name");
        consume(Token_Type::LEFT_PAREN, "Expected '(' after " + kind + " name." );
        auto params = std::vector<Token>{};
        if(not match(Token_Type::RIGHT_PAREN)){
            auto first_param = consume(Token_Type::IDENTIFIER, "Expected paramater name");
            params.push_back(first_param);

            while(match(Token_Type::COMMA)){
                if(params.size() >= 255){
                    error(peek(), "Can't have more than 255 paramters");
                }
                auto param = consume(Token_Type::IDENTIFIER, "Expected paramter name" );
                params.push_back(param);
            }

             consume(Token_Type::RIGHT_PAREN, "Expected ')' after " + kind + " arguments.");
        }
        
        consume(Token_Type::LEFT_BRACE, "Expected '{' before " + kind + " body.");

        auto body = block_statement();

        return Box<Function>{name, params, body};
    }


    auto Parser::parse() -> std::vector<Stmt>{
        auto statements = std::vector<Stmt>{};
        while(!is_at_end()){
            statements.push_back(declaration());
        }
        return statements;
    }
    
    
    
};