cmake_minimum_required(VERSION 3.28)

project(lox VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lox 
			  src/main.cpp
			  src/scanner.cpp 
			  src/token.cpp 
			  src/error.cpp
			  )


target_include_directories(lox PUBLIC
						   "${PROJECT_SOURCE_DIR}/src")



