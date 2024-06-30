cmake_minimum_required(VERSION 3.28)

project(lox 
	LANGUAGES CXX
	VERSION 0.1
	)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")


add_subdirectory(src)


# Including custom cmake rules
include(cmake/clang-cxx-dev-tools.cmake)








