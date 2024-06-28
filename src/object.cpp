#include "object.hpp"
#include "interpreter.hpp"
#include "expr.hpp"

namespace lox {
    Func::Func(std::function<auto () -> int> const& t_arity, std::function<auto (Interpreter&, std::vector<Object>&) -> Object> const& t_call, std::string const& t_name)
    :arity{t_arity}
    ,call{t_call}
    ,name{t_name} {}

        
    Lox_Function::Lox_Function(std::any t_declaration, std::shared_ptr<Enviroment> t_closure)
    :m_declaration{t_declaration}
    ,m_closure{t_closure} {}

    auto Lox_Function::arity() -> std::size_t{
        auto declaration = std::any_cast<Box<Function>>(m_declaration);
        return declaration->m_params.size();
    }

    auto Lox_Function::call(Interpreter& interpreter, std::vector<Object>& arguments) -> Object{
        auto declaration =  std::any_cast<Box<Function>>(m_declaration);
        auto enviroment = std::make_shared<Enviroment>(m_closure);
        for(int i = 0; i < declaration->m_params.size(); i++){
            enviroment->define(declaration->m_params[i].get_lexume(), arguments[i] );
        }

        try{
            interpreter.execute_block(declaration->m_body,enviroment);
        }catch(Object& o){
            return o;
        }
        
        return {};
    }
};