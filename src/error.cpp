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

	auto error(Token token, std::string const& message) -> void{
		if(token.get_token_type() == Token_Type::END_OF_FILE){
			report(token.get_line(), " at end ", message);
		}else{
			report(token.get_line(), " at '" + token.get_lexume() + "'", message);
		}
	}
};