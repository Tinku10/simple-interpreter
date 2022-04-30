#include <iostream>

#include "Interpreter.h"
#include "Parser.h"

int main() {
  std::string source
      = "BEGIN"
        "  BEGIN"
        "      number := 2;"
        "      a := number;"
        "      b := 10 * a + 10 * number / 4;"
        "      c := a - - b"
        "  END;"
        "  x := 11;"
        "END.";

  Lexer lexer(source);
  Parser parser(lexer);
  Visitor visitor;

  Interpreter interpreter(parser, visitor);
  interpreter.interpret();
}
