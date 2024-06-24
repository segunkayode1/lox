cmake_minimum_required(VERSION 3.28)

project(lox 
	LANGUAGES CXX
	VERSION 0.1
	)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)










