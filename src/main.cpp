#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <span>
#include <string>
#include <vector>

#include "error.hpp"
#include "interpreter.hpp"
#include "parser.hpp"
#include "resolver.hpp"
#include "scanner.hpp"

namespace {
constexpr int EX_BASE = 64;
constexpr int EX_DATAERR = 65;
constexpr int EX_SOFTWARE = 70;

auto run(std::string const &source) -> void {
  auto scanner = lox::Scanner(source);
  auto tokens = scanner.scan_tokens();
  if (lox::had_error) {
    return;
  }
  auto parser = lox::Parser(tokens);
  auto statements = parser.parse();
  if (lox::had_error) {
    return;
  }

  // std::cout << lox::Ast_Printer{}.print(ast) << '\n';
  auto static interpreter = lox::Interpreter{};

  auto resolver = lox::Resolver{interpreter};
  resolver.resolve(statements);

  if (lox::had_error) {
    return;
  }

  interpreter.interpret(statements);
}

auto run_file(std::string const &path) -> void {
  auto file = std::ifstream{path};
  if (file.fail()) {
    std::cerr << "file: " << path << " does not exist\n";
    exit(EX_DATAERR);
  }
  auto const source = std::string{std::istreambuf_iterator<char>{file},
                                  std::istreambuf_iterator<char>{}};
  run(source);
  if (lox::had_error) {
    std::exit(EX_DATAERR);
  }
  if (lox::had_runtime_error) {
    std::exit(EX_SOFTWARE);
  }
}

auto run_prompt() -> void {
  std::cout << "lox> ";
  auto line = std::string();
  while (getline(std::cin, line)) {
    run(line);
    lox::had_error = false;
    std::cout << "lox> ";
  }
}
}; // namespace

auto main(int argc, char *argv[]) -> int {
  auto const args = std::span(argv, static_cast<std::size_t>(argc));
  if (args.size() > 2) {
    std::cerr << "Usage: lox [script]\n";
    std::exit(EX_BASE);
  } else if (args.size() == 2) {
    run_file(args[1]);
  } else {
    run_prompt();
  }
  return 0;
}
