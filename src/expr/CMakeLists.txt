add_library(expr ast_printer.cpp expr.cpp)

target_include_directories(expr INTERFACE 
                          "${CMAKE_CURRENT_SOURCE_DIR}")