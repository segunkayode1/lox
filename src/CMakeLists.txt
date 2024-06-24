add_executable(lox error.cpp parser.cpp scanner.cpp token.cpp main.cpp interpreter.cpp)

add_subdirectory(expr)

target_link_libraries(lox PRIVATE expr)