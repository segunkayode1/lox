#include <variant>
#include <iostream>
#include <sstream>

#include "ast_printer.hpp"


namespace lox {
    template<typename... T>
    auto Ast_Printer::parenthisize(std::string name, T... exprs) -> std::string{
        auto result = std::stringstream{};
        result << "(" << name;
        for(auto expr : {exprs...}){
            result << " ";
            result << std::visit(*this, expr);
        }
        result << ")";
        return result.str();
    }

    auto Ast_Printer::print(Expr& expr) -> std::string{
        return std::visit(*this, expr);
    }
    auto Ast_Printer::operator()(std::monostate /*unused*/) -> std::string{
        return {};
    }

    auto Ast_Printer::operator()(Box<Binary>& expr) -> std::string{
        return parenthisize(expr->m_operator.get_lexume(), expr->m_left, expr->m_right);
    }

    auto Ast_Printer::operator()(Box<Grouping>& expr) -> std::string{
        return parenthisize("group", expr->m_expression);
    }
    auto Ast_Printer::operator()(Box<Literal>& expr) -> std::string{\
        auto value = expr->m_value;
        if(auto x = std::any_cast<std::string>(&value)){
           return std::any_cast<std::string>(value);
        }

        if(auto x = std::any_cast<double>(&value)){
            return std::to_string(std::any_cast<double>(value));
        }

        return "nill";
    }
    auto Ast_Printer::operator()(Box<Unary>& expr) -> std::string{
        return parenthisize(expr->m_operator.get_lexume(), expr->m_right);
    }
}


int main(){
    using namespace lox;
    Expr expression  = Box<Binary>{
            Box<Unary>{
                    Token{Token_Type::MINUS, "-", std::any{}, 1}
                    ,Box<Literal>{
                            123.0
                    }
            }
            ,Token{Token_Type::STAR, "*", std::any{}, 1}
            ,Box<Grouping>{
                    Box<Literal>{
                            45.67
                    }
            }
    };

    std::cout << Ast_Printer{}.print(expression) << '\n';
}