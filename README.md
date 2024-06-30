# Lox

A C++ implementation of jlox from Part I of [Crafting Interpreters](https://www.craftinginterpreters.com/), written in C++20 with a focus on modern C++ practices.

## Getting Started

### Prerequisites
- A C++20 compatible compiler (e.g., GCC 10+, Clang 10+, MSVC 2019+)
- CMake

### Building

1. Clone the repository:
    ```bash
    $ git clone https://github.com/segunkayode1/lox.git
    $ cd lox
    ```

2. Create and navigate to the build directory:
    ```bash
    $ mkdir build
    $ cd build
    ```

3. Run CMake and build the project:
    ```bash
    $ cmake ..
    $ cmake --build .
    ```

### Running

You can either give a file to be interpreted:  
 ```bash
   $ lox /path/to/your/file.lox
 ```

Or start an interactive session:  
 ```bash
   $ lox
 ```

## Status of Project

- Chapter 4
- Chapter 5
- Chapter 6
- Chapter 7
- Chapter 8
- Chapter 9
- Chapter 10
- Chapter 11
- Chapter 12 (**in progress**)

## Acknowledgements

- Bob Nystrom, for his excellent book [Crafting Interpreters](https://www.craftinginterpreters.com/).

## About the Book
>Crafting Interpreters contains everything you need to implement a full-featured, efficient scripting language. You’ll learn both high-level concepts around parsing and semantics and gritty details like bytecode representation and garbage collection. Your brain will light up with new ideas, and your hands will get dirty and calloused. It’s a blast.
