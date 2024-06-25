#include "enviroment.hpp"
#include "error.hpp"

namespace lox{
    
    Enviroment::Enviroment(std::unique_ptr<Enviroment> t_parent)
    :m_parent{std::move(t_parent)} {}

    auto Enviroment::define(std::string const& name, Object const& value) -> void{
        m_values[name] = value;
    }

    auto Enviroment::assign(Token const& name, Object const& value) -> void{
        if(m_values.contains(name.get_lexume())){
             m_values[name.get_lexume()] = value;
             return;
        }

        if(m_parent){
            m_parent->assign(name, value);
            return;
        }

        throw Runtime_Error{name, "Undefined Indentifier : '"+ name.get_lexume() +"'."};
    }

    auto Enviroment::get(Token const& name) -> Object{
        if(m_values.contains(name.get_lexume())){
            return m_values[name.get_lexume()];
        }

        if(m_parent){
            return m_parent->get(name);
        }

        throw Runtime_Error{name, "Undefined Indentifier : '"+ name.get_lexume() +"'."};
    }

     auto Enviroment::get_parent() -> std::unique_ptr<Enviroment>{
        return std::move(m_parent);
     }
}