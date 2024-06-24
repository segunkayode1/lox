#ifndef LOX_ERROR_HPP
#define LOX_ERROR_HPP

#include <string>
namespace lox {
    extern bool had_error;
    auto report(int line , std::string const& where, std::string const& message) -> void;
    auto error(int line , std::string const& message) -> void;
}; //lox

#endif