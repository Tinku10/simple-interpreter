#include <iostream>

#include "Interpreter.h"
#include "Parser.h"

int main() {
  std::string source
    ="PROGRAM Main;"
     "   VAR x, y: REAL;"

     "   PROCEDURE Alpha(a : INTEGER);"
     "      VAR y : INTEGER;"
     "   BEGIN"

     "   END;"

     "BEGIN { Main }"

     "END.  { Main }";

  Lexer lexer(source);
  Parser parser(lexer);
  NodeVisitor visitor;
  SymbolTableVisitor symbol_visitor;

  Interpreter interpreter(parser, visitor, symbol_visitor);
  interpreter.interpret();
}
