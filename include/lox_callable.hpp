#ifndef LOX_CALLABLE_HPP
#define LOX_CALLABLE_HPP

#include <variant>
#include <vector>

#include "object.hpp"
#include "interpreter.hpp"

namespace lox{
    struct Caller{
        Caller(Interpreter& t_interpreter, std::vector<Object>& t_arguments);
        auto call(Lox_Callable& callee) -> Object;
        auto operator()(std::monostate /*unused*/) -> Object;
        auto operator()(auto& callee) -> Object;
        private:
        Interpreter& m_interpreter;
        std::vector<Object>& m_arguments;
    };


    struct Arity{
        auto arity(Lox_Callable& callee) -> std::size_t;
        auto operator()(std::monostate /*unused*/) -> std::size_t;
        auto operator()(auto& callee) -> std::size_t;
    };


};

#endif