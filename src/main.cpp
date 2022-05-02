#include <iostream>

#include "Interpreter.h"
#include "Parser.h"

int main() {
  std::string source
    =  "PROGRAM Part12;"
       "VAR"
       "   a : INTEGER;"
       ""
       "PROCEDURE P1;"
       "VAR"
       "   a : REAL;"
       "   k : INTEGER;"
       ""
       "   PROCEDURE P2;"
       "   VAR"
       "      a, z : INTEGER;"
       "   BEGIN {P2}"
       "      z := 777;"
       "   END;  {P2}"
       ""
       "BEGIN {P1}"
       "END;  {P1}"
       ""
       "BEGIN {Part12}"
       "   a := 10;"
       "END.  {Part12}";

  Lexer lexer(source);
  Parser parser(lexer);
  NodeVisitor visitor;
  SymbolTableVisitor symbol_visitor;

  Interpreter interpreter(parser, visitor, symbol_visitor);
  interpreter.interpret();
}
