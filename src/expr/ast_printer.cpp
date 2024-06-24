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
        if(std::holds_alternative<std::string>(value)){
           return std::get<std::string>(value);
        }

        if(std::holds_alternative<double>(value)){
            return std::to_string(std::get<double>(value));
        }

        if(std::holds_alternative<bool>(value)){
            return std::get<bool>(value) ? "true" : "false";
        }

        return "nill";
    }
    auto Ast_Printer::operator()(Box<Unary>& expr) -> std::string{
        return parenthisize(expr->m_operator.get_lexume(), expr->m_right);
    }
}


// int main(){
//     using namespace lox;
//     Expr expression  = Box<Binary>{
//         Box<Unary>{
//             Token(Token_Type::MINUS, "-", Object{}, 1),
//             Box<Literal>{
//                 123.0
//             }
//         }
//         ,Token(Token_Type::STAR, "*", Object{}, 1)
//         ,Box<Grouping>{
//             Box<Literal>{
//                 45.57
//             }
//         }
//     };

//     std::cout << Ast_Printer{}.print(expression) << '\n';
// }