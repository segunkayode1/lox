#include "lox_callable.hpp"


namespace lox{
    Caller::Caller(Interpreter& t_interpreter, std::vector<Object>& t_arguments)
    :m_interpreter{t_interpreter}
    ,m_arguments{t_arguments} {}

    auto Caller::call(Lox_Callable& callee) -> Object{
        return std::visit(*this, callee);
    }
    auto Caller::operator()(std::monostate /*unused*/) -> Object{
        return {};
    }

    auto Arity::arity(Lox_Callable& callee) -> std::size_t{
        return std::visit(*this, callee);
    }

    auto Arity::operator()(std::monostate /*unused*/) -> std::size_t{
        return 0u;
    }

    auto Caller::operator()(auto& callee) -> Object{
        return callee.call(m_interpreter, m_arguments);
    }

    auto Arity::operator()(auto& callee)-> std::size_t{
        return callee.arity();
    }
   
}