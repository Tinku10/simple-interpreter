#include <iostream>

#include "Interpreter.h"
#include "Parser.h"
#include <sstream>
#include <fstream>

int main() {
  std::stringstream source;

  std::ifstream file("../src/source.pas");

  if(!file.is_open()) {
    std::cout << "cannot open file\n";
  }

  std::string line;

  while(std::getline(file, line)) {
    source << line + "\n";
  }

  file.close();

  std::string code = source.str();

  Lexer lexer(code);
  Parser parser(lexer);

  Interpreter interpreter(parser);
  interpreter.interpret();
}
