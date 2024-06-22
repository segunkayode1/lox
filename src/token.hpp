#ifndef LOX_TOKEN_HPP
#define LOX_TOKEN_HPP

#include <string>
#include <any>

#include "token_type.hpp"
namespace lox{
    class Token{    
        public:
        Token(Token_Type t_type, std::string const& t_lexume, std::any const& t_literal, int t_line );
        auto to_string() const -> std::string;

        auto get_token() const -> Token_Type;
        auto get_lexume() const -> std::string const&;
        auto get_literal() const -> std::any const&;
        auto get_line() const -> int;

        private:
        auto literal_to_string() const -> std::string;

        Token_Type const m_type;
        std::string const m_lexume;
        std::any const m_literal;
        int const m_line;
    };
};
#endif