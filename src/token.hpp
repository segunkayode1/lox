#ifndef LOX_TOKEN_HPP
#define LOX_TOKEN_HPP

#include <string>
#include <variant>

#include "token_type.hpp"
namespace lox{
    using Object = std::variant<double,int, std::string, std::monostate>;
    class Token{    
        public:
        Token(Token_Type t_type, std::string const& t_lexume, Object const& t_literal, int t_line );
        auto to_string() const -> std::string;

        auto get_token() const -> Token_Type;
        auto get_lexume() const -> std::string const&;
        auto get_literal() const -> Object const&;
        auto get_line() const -> int;

        private:
        auto literal_to_string() const -> std::string;

        Token_Type m_type;
        std::string m_lexume;
        Object m_literal;
        int m_line;
    };
};
#endif