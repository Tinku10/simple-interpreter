#include "Parser.h"

class Interpreter {
 public:
  Parser parser;

  Interpreter(Parser& parser);

  void interpret();
};
