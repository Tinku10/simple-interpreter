#include <iostream>

#include "Interpreter.h"
#include "Parser.h"

int main() {
  std::string source
      = "PROGRAM Main;"
        "   VAR x, y : REAL;"
        "   VAR z : INTEGER;"

        "   PROCEDURE AlphaA(a : INTEGER);"
        "      VAR y : INTEGER;"
        "   BEGIN { AlphaA }"
        "      x := a + x + y;"
        "   END;  { AlphaA }"

        "   PROCEDURE AlphaB(a : INTEGER);"
        "      VAR b : INTEGER;"
        "   BEGIN { AlphaB }"
        "   END;  { AlphaB }"

        "BEGIN { Main }"
        "END.  { Main }";

  Lexer lexer(source);
  Parser parser(lexer);

  Interpreter interpreter(parser);
  interpreter.interpret();
}
