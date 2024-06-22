#include <iostream>

#include "error.hpp"

namespace lox {
    bool had_error = false;

    auto report(int line , std::string const& where, std::string const& message) -> void{
		std::cerr << "[line " << line << "] Error" << where <<  ": " << message << '\n';
		had_error = true;
	}

	auto error(int line , std::string const& message) -> void{
		report(line, "", message);
	}
}