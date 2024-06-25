#ifndef LOX_ENV_HPP
#define LOX_ENV_HPP
#include <unordered_map>
#include <memory>

#include "token.hpp"
namespace lox{
    class Enviroment{
        public:
        Enviroment() = default;
        Enviroment(std::unique_ptr<Enviroment> t_parent);
        auto define(std::string const& name, Object const& value) -> void;
        auto assign(Token const& name, Object const& value) -> void;
        auto get(Token const& name) -> Object;
        auto get_parent() -> std::unique_ptr<Enviroment>;
        private:
        std::unordered_map<std::string, Object> m_values;
        std::unique_ptr<Enviroment> m_parent;
    };

};

#endif