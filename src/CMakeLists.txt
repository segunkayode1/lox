add_executable(lox error.cpp parser.cpp scanner.cpp token.cpp main.cpp interpreter.cpp enviroment.cpp)

add_subdirectory(expr)
add_subdirectory(stmt)

target_include_directories(lox PUBLIC
     ${PROJECT_SOURCE_DIR}/include
     ${PROJECT_SOURCE_DIR}/include/expr
     ${PROJECT_SOURCE_DIR}/include/stmt)