#include <sstream>
#include <string> 
#include <any>

#include "token.hpp"
#include "magic_enum.hpp"

namespace lox{
    Token::Token(Token_Type t_type, std::string const& t_lexume, std::any const& t_literal, int t_line )
    :m_type{t_type}
    ,m_lexume{t_lexume}
    ,m_literal{t_literal}
    ,m_line{t_line}{};
    
    auto Token::literal_to_string() const -> std::string{
        switch(m_type){
            case Token_Type::STRING:
                return std::any_cast<std::string>(m_literal);
            case Token_Type::NUMBER:
                return std::to_string(std::any_cast<double>(m_literal));
            default:
                return "";
        }
    }

    auto Token::to_string() const -> std::string{
        auto ss = std::stringstream{};
        ss << "Token Type: " << magic_enum::enum_name(m_type);
        ss << " ,Lexume: " << m_lexume;
        ss << " ,Literal: " << literal_to_string();
        ss << " ,Line Number: " << m_line << '\n';
        return ss.str();
    }

    auto Token::get_token() const -> Token_Type{
        return m_type;
    }
    auto Token::get_lexume() const -> std::string const&{
        return m_lexume;
    }
    auto Token::get_literal() const -> std::any const&{
        return m_literal;
    }
    auto Token::get_line() const -> int{
        return m_line;
    }   
}