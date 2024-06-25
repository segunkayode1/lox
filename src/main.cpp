#include <iostream> 
#include <fstream>
#include <string>
#include <iterator>
#include <vector>

#include "error.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "ast_printer.hpp"
#include "interpreter.hpp"

namespace {
	auto run(std::string const& source) -> void {
		auto scanner = lox::Scanner(source);
		auto tokens = scanner.scan_tokens();
		if(lox::had_error) return;
		auto parser  = lox::Parser(tokens);
		auto statements = parser.parse();
		if(lox::had_error) return;
		// std::cout << lox::Ast_Printer{}.print(ast) << '\n';
        static lox::Interpreter interpreter;
		interpreter.interpret(statements);
	}

	auto run_file(std::string const& path) -> void{
		auto file = std::ifstream{path};
		auto const source = std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
		run(source);
		if (lox::had_error){
			std::exit(65);
		}
		if(lox::had_runtime_error){
			std::exit(70);
		}
	}

	auto run_prompt() -> void{
		std::cout << "lox> ";
		auto line = std::string();
		while(getline(std::cin, line)){
			run(line);
			lox::had_error = false;
			std::cout << "lox> ";
		}
	}
};

auto main(int argc, char** argv) -> int{
	if(argc > 2){
		std::cerr << "Usage: lox [script]\n";
		std::exit(64);
	}else if( argc == 2){
		run_file(argv[1]);
	}else{
		run_prompt();
	}
}
