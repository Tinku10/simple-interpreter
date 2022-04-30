#include "Parser.h"

class Interpreter {
public:
  Parser parser;
  Visitor visitor;

  Interpreter(Parser& parser, Visitor& visitor);

  void interpret();
};
