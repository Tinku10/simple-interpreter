cmake_minimum_required(VERSION 3.13)

project(interpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17 CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/Token.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/NodeVisitor.cpp
  src/SymbolTableVisitor.cpp
  src/Interpreter.cpp
  src/ScopedSymbolTable.cpp
  src/Symbol.cpp
  src/SourceToSourceCompilerVisitor.cpp
)

# generate compiler commands to help clangd find include paths
# even if not set, there is no error in compilation and linking but neovim diagnostics shows error
set(DCMAKE_EXPORT_COMPILE_COMMANDS 1 CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 17)
