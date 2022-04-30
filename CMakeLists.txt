cmake_minimum_required(VERSION 3.13)

project(interpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/Token.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/Visitor.cpp
  src/Interpreter.cpp
)
# generate compiler commands to help clangd find include paths
# even if not set, there is no error in compilation and linking but neovim diagnostics shows error
set(DCMAKE_EXPORT_COMPILE_COMMANDS 1 CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 17)
