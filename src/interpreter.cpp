#include <variant>
#include <iostream>

#include "interpreter.hpp"
#include "token.hpp"
#include "error.hpp"

namespace lox{

    auto to_string_object(Object o) -> std::string{
        if(std::holds_alternative<bool>(o)){
            return std::get<bool>(o) ? "true" : "false";
        }
        if(std::holds_alternative<std::string>(o)){
            return std::get<std::string>(o);
        }

        if(std::holds_alternative<double>(o)){
            auto d = std::get<double>(o);
            if(d == static_cast<int>(d)){
                return std::to_string(static_cast<int>(d));
            }
            return std::to_string(d);
        }

        return "nil";
    }

    auto Interpreter::interpret(Expr expr) -> void{
        try{
            Object o = evaluate(expr);
            std::cout << to_string_object(o) << '\n';
        }catch(Runtime_Error& err){
            runtime_error(err);
        }
    }
    auto Interpreter::evaluate(Expr expr) -> Object{
         return std::visit(*this,expr);
    }
    auto Interpreter::operator()(std::monostate /*unused*/) -> Object{
          return {};
    }

    auto is_equal(Object const& left, Object const& right){
        auto both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);
        auto both_strings = std::holds_alternative<std::string>(left) and std::holds_alternative<std::string>(right);
        auto both_bools = std::holds_alternative<bool>(left) and std::holds_alternative<bool>(right);
        auto both_nil = std::holds_alternative<std::monostate>(left) and std::holds_alternative<std::monostate>(right);
        if(both_numbers){
            return std::get<double>(left) ==  std::get<double>(right);
        }
        if(both_strings){
            return std::get<std::string>(left) ==  std::get<std::string>(right);
        }
        if(both_bools){
            return std::get<bool>(left) ==  std::get<bool>(right);
        }
        return both_nil;
    }

    auto Interpreter::operator()(Box<Binary>& expr) -> Object{
        auto left = evaluate(expr->m_left);
        auto right = evaluate(expr->m_right);
        using enum Token_Type;
        auto both_numbers = false;
        auto both_strings = false;
        switch(expr->m_operator.get_token_type()){
            case MINUS:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) - std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            case PLUS:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            both_strings = std::holds_alternative<std::string>(left) and std::holds_alternative<std::string>(right);

            if(both_numbers){
                return std::get<double>(left) + std::get<double>(right);
            }

            if(both_strings){
                return std::get<std::string>(left) + std::get<std::string>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers or strings"};

            case EQUAL_EQUAL:
            return is_equal(left, right);

            case BANG_EQUAL:
            return not is_equal(left, right);

            case GREATER:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) > std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            case GREATER_EQUAL:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) >= std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };
            
            case LESS:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) < std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };
    
            case LESS_EQUAL:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) <= std::get<double>(right);
            }
        
            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            case STAR:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) * std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };
            case SLASH:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);
            if(both_numbers){
                return std::get<double>(left) / std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            default:
            return {};
        }
        return {};
    }

    auto Interpreter::operator()(Box<Grouping>& expr) -> Object{
        return evaluate(expr->m_expression);
    }

    auto Interpreter::operator()(Box<Literal>& expr) -> Object{
        return expr->m_value;
    }
   
    auto truthy(Object o){
        if(std::holds_alternative<std::monostate>(o)){
            return false;
        }
        if(std::holds_alternative<bool>(o)){
            return std::get<bool>(o);
        }
        return true;
    }

    auto Interpreter::operator()(Box<Unary>& expr) -> Object{
        auto value = evaluate(expr->m_right);
        switch(expr->m_operator.get_token_type()){
            case Token_Type::MINUS:
                if(not std::holds_alternative<double>(value)){
                    throw Runtime_Error{expr->m_operator, "operand has to a number" };
                }
                return std::get<double>(value) * -1;
            case Token_Type::BANG:
                return not truthy(value);       
            default:
                throw Runtime_Error{expr->m_operator, "Parser Error" };
        }
    }

}